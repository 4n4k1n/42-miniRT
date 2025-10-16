/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   master.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 11:41:09 by anakin            #+#    #+#             */
/*   Updated: 2025/10/12 13:24:17 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static int setup_master(t_master *master, t_data *data, char *scene_file, uint32_t port)
{
    ft_memset(master, 0, sizeof(t_master));
    master->scene_file = scene_file;
    master->start_render = false;
    parse_scene(scene_file, data);
    master->queue = malloc(sizeof(t_queue));
    if (!master->queue)
        return (ft_error("malloc", 1));
    init_queue(master->queue, WIDTH, HEIGHT, TILE_SIZE);
    master->mlx = mlx_init(WIDTH, HEIGHT, "miniRT", false);
    if (!master->mlx)
        return (ft_error("mlx_init", 1));
    master->img = mlx_new_image(master->mlx, WIDTH, HEIGHT);
    if (!master->img)
        return (ft_error("mlx_new_image", 1));
    mlx_image_to_window(master->mlx, master->img, 0, 0);
    pthread_mutex_init(&master->img_lock, NULL);
    master->socket_fd = setup_listen_socket(port);
    if (master->socket_fd < 0)
        return (1);
    pthread_create(&master->accept_thread, NULL, accept_worker_threads, master);
    return (0);
}

int run_master(char *scene_file, uint32_t port)
{
    t_master    master;
    t_data      data;
    char        buffer[10];
    int         prev_workers;

    if (setup_master(&master, &data, scene_file, port) != 0)
        return (1);
    printf("\n=== Master Server Ready ===\n");
    printf("Scene: %s\n", scene_file);
    printf("Resolution: %dx%d\n", (int)WIDTH, (int)HEIGHT);
    printf("Tile size: %d\n", TILE_SIZE);
    printf("Total tiles: %zu\n\n", master.queue->size);
    printf("Waiting for workers to connect...\n");
    printf("Press ENTER to start rendering\n\n");
    prev_workers = 0;
    while (1)
    {
        fd_set readfds;
        struct timeval tv;
        int ret;

        if (master.num_worker != prev_workers)
        {
            printf("Workers connected: %d\n", master.num_worker);
            prev_workers = master.num_worker;
        }
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        tv.tv_sec = 0;
        tv.tv_usec = 100000;
        ret = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv);
        if (ret > 0 && FD_ISSET(STDIN_FILENO, &readfds))
        {
            if (fgets(buffer, sizeof(buffer), stdin) != NULL && buffer[0] == '\n')
                break;
        }
    }
    printf("\n=== Starting Render ===\n");
    printf("Workers: %d\n", master.num_worker);
    printf("Tiles per worker: ~%zu\n\n", master.queue->size / (master.num_worker > 0 ? master.num_worker : 1));
    master.start_render = true;
    mlx_loop(master.mlx);
    master.shutdown = true;
    pthread_join(master.accept_thread, NULL);
    destroy_queue(master.queue);
    free(master.queue);
    mlx_terminate(master.mlx);
    free_scene(&data);
    return (0);
}
