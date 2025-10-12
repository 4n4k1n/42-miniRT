/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worker.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 13:35:40 by anakin            #+#    #+#             */
/*   Updated: 2025/10/12 16:03:20 by anakin           ###   ########.fr       */
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
    
    printf("Worker connected from socket: %d\n", context->worker_socket);
    send_file(master->scene_file, context->worker_socket);
    header = recive_header(context->worker_socket);
    if (header.msg_type != MSG_WORKER_READY)
        return (close(context->worker_socket), NULL);
    while (!master->shutdown)
    {
        if (!queue_next_job(&master->queue, &tile)) 
        {
            send_header(context->worker_socket, MSG_SHUTDOWN, 0);
            break ;
        }
        send_tile_assignment(context->worker_socket, &tile);
        recive_tile_result(context->worker_socket, &result, &pixels);
        pthread_mutex_lock(&master->img_lock);
        // copie tile to img
        pthread_mutex_unlock(&master->img_lock);
        free(pixels);
        printf("Tile %d completed (%d/%d)\n", result.tile_id, master->queue->current, master->queue->size);
    }
    close(context->worker_socket);
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
