/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worker.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apregitz <apregitz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 13:35:40 by anakin            #+#    #+#             */
/*   Updated: 2025/10/25 14:07:55 by apregitz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

void    *worker_thread_func(void *arg)
{
    t_worker_context    *context;
    t_master            *master;
    t_msg_header        header;
    t_tile              tile;
    t_tile              result;
    uint32_t            *pixels;
    t_settings          settings;

    context = (t_worker_context *)arg;
    master = context->master;
    printf("Worker connected from socket: %d\n", context->worker_socket);
    register_worker(master, context->worker_socket);
    send_file(master->scene_file, context->worker_socket);
    settings.aa_state = ANTI_ALIASING;
    settings.depth = MAX_DEPTH;
    settings.light_state = false;
    settings.max_samples = AA_MAX_SAMPLES;
    settings.min_samples = AA_MIN_SAMPLES;
    settings.scale = SCALE;
    settings.shadow_samples = SHADOW_SAMPLES;
    send_settings(master->socket_fd, &settings);
    header = recive_header(context->worker_socket);
    if (header.msg_type != MSG_WORKER_READY)
    {
        unregister_worker(master, context->worker_socket);
        return (close(context->worker_socket), free(context), NULL);
    }
    printf("Worker %d ready, waiting for render signal...\n", context->worker_socket);
    while (!master->start_render && !master->shutdown)
        usleep(100000);
    if (master->shutdown)
    {
        unregister_worker(master, context->worker_socket);
        return (close(context->worker_socket), free(context), NULL);
    }
    printf("Worker %d starting render\n", context->worker_socket);
    while (!master->shutdown)
    {
        while (!master->shutdown)
        {
            if (!queue_next_job(master->queue, &tile))
                break ;
            send_tile_assignment(context->worker_socket, &tile);
            recive_tile_result(context->worker_socket, &result, &pixels);
            pthread_mutex_lock(&master->img_lock);
            copy_tile_to_framebuffer(master->img, &result, pixels);
            pthread_mutex_unlock(&master->img_lock);
            free(pixels);
            printf("Tile %d completed (%zu/%zu)\n", result.tile_id, master->queue->current, master->queue->size);
        }
        printf("Worker %d finished rendering, waiting for updates...\n", context->worker_socket);
        while (!master->shutdown)
        {
            pthread_mutex_lock(&master->restart_lock);
            if (master->restart_render)
            {
                pthread_mutex_unlock(&master->restart_lock);
                printf("Worker %d restarting render...\n", context->worker_socket);
                break;
            }
            pthread_mutex_unlock(&master->restart_lock);
            usleep(100000);
        }
    }
    unregister_worker(master, context->worker_socket);
    send_header(context->worker_socket, MSG_SHUTDOWN, 0);
    close(context->worker_socket);
    free(context);
    return (NULL);
}

void copy_tile_to_framebuffer(mlx_image_t *image, t_tile *result, uint32_t *pixels)
{
    uint32_t    y;
    uint32_t    x;
    uint32_t img_x;
    uint32_t img_y;
    uint32_t pixel_idx;
    uint32_t img_idx;

    y = 0;
    while (y < result->height)
    {
        x = 0;
        while (x < result->width)
        {
            img_x = result->x + x;
            img_y = result->y + y;
            pixel_idx = y * result->width + x;
            img_idx = img_y * image->width + img_x;
            image->pixels[img_idx * 4 + 0] = (pixels[pixel_idx] >> 24) & 0xFF;
            image->pixels[img_idx * 4 + 1] = (pixels[pixel_idx] >> 16) & 0xFF;
            image->pixels[img_idx * 4 + 2] = (pixels[pixel_idx] >> 8) & 0xFF;
            image->pixels[img_idx * 4 + 3] = pixels[pixel_idx] & 0xFF;
            x++;
        }
        y++;
    }
}

void    *accept_worker_threads(void *arg)
{
    t_master    *master;
    int         worker_socket;
    t_worker_context    *context;
    pthread_t           thread;

    master = (t_master *)arg;
    while (!master->shutdown)
    {
        worker_socket = accept(master->socket_fd, NULL, NULL);
        if (worker_socket < 0)
            continue ;
        if (master->num_worker >= MAX_WORKER)
        {
            close(worker_socket);
            continue ;
        }
        context = malloc(sizeof(t_worker_context));
        if (!context)
            return (close(worker_socket), ft_error("malloc", 1), NULL);
        context->master = master;
        context->worker_socket = worker_socket;
        pthread_create(&thread, NULL, worker_thread_func, context);
        pthread_detach(thread);
        master->num_worker++;
    }
    return (NULL);
}

int run_worker(char *master_ip, uint32_t port)
{
    int master_socket;
    struct sockaddr_in master_addr;
    char *scene_content;
    int scene_file_fd;
    t_data  data;
    uint32_t    tiles_rendered;
    t_msg_header    header;
    t_tile          tile;
    uint32_t        *pixels;
    t_tile          result;

    master_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (master_socket < 0)
        return (ft_error("socket", 1));
    master_addr.sin_family = AF_INET;
    master_addr.sin_port = htons(port);
    inet_pton(AF_INET, master_ip, &master_addr.sin_addr);
    printf("Connecting to master %s:%d\n", master_ip, (int)port);
    if (connect(master_socket, (struct sockaddr *)&master_addr, sizeof(master_addr)) < 0)
        return (ft_error("connect", 1));
    printf("Connected");
    scene_content = recive_scene_file(master_socket);
    if (!scene_content)
    {
        printf("failed to recive scene\n");
        return (1);
    }
    scene_file_fd = open("scene.rt", O_WRONLY | O_CREAT | O_TRUNC, 0777);
    if (scene_file_fd < 0)
        return (ft_error("open", 1));
    write(scene_file_fd, scene_content, ft_strlen(scene_content));
    close(scene_file_fd);
    free(scene_content);
    parse_scene("scene.rt", &data);
    data.camera.samples_per_pixel = AA_MAX_SAMPLES;
    init_camera(&data);
    init_threads_worker(&data);
    data.settings = recive_settings(master_socket);
    data.camera.samples_per_pixel = data.settings.min_samples;
    send_header(master_socket, MSG_WORKER_READY, 0);
    printf("ready to render\n");
    tiles_rendered = 0;
    while (true)
    {
        header = recive_header(master_socket);
        if (header.msg_type == MSG_SHUTDOWN)
        {
            printf("recived shutdown\n");
            break;
        }
        if (header.msg_type == MSG_UPDATE)
        {
            t_camera_update cam_update;
            recv_all(master_socket, &cam_update, sizeof(t_camera_update));
            data.camera.cords.x = cam_update.x;
            data.camera.cords.y = cam_update.y;
            data.camera.cords.z = cam_update.z;
            data.camera.pitch = cam_update.pitch;
            data.camera.yaw = cam_update.yaw;
            data.settings.aa_state = cam_update.aa_state;
            data.settings.light_state = cam_update.light_state;
            update_camera(&data);
            printf("Received camera update from master, ready for new render...\n");
            continue;
        }
        if (header.msg_type != MSG_RENDER_TILE)
        {
            printf("unexpected message type: %d\n", header.msg_type);
            break;
        }
        tile = recive_tile_assignment(master_socket);
        printf("Rendering tile %d (%dx%d at %d,%d)...\n", tile.tile_id, tile.width, tile.height, tile.x, tile.y);
        pixels = render_tile(&data, &tile);
        if (!pixels)
        {
            printf("Failed to render tile %d\n", tile.tile_id);
            break;
        }
        result.height = tile.height;
        result.tile_id = tile.tile_id;
        result.width = tile.width;
        result.x = tile.x;
        result.y = tile.y;
        send_tile_result(master_socket, &result, pixels);
        free(pixels);
        tiles_rendered++;
        printf("Tile %d completed (total: %d)\n", tile.tile_id, tiles_rendered);
    }
    printf("Worker shutting down. Total tiles rendered: %d\n", tiles_rendered);
    close(master_socket);
    free_scene(&data);
    return (0);
}
