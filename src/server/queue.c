/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 23:17:51 by anakin            #+#    #+#             */
/*   Updated: 2025/10/12 00:20:09 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

void    init_queue(t_queue *queue, uint32_t width, uint32_t height, uint32_t tile_size)
{
    uint32_t    tiles_x;
    uint32_t    tiles_y;
    uint32_t    i;
    uint32_t    j;
    uint32_t    tile_id;

    pthread_mutex_init(&queue->lock, NULL);
    tiles_x = (width + tile_size - 1) / tile_size;
    tiles_y = (height + tile_size - 1) / tile_size;
    queue->size = tiles_x * tiles_y;
    queue->tiles = malloc(sizeof(t_tile) * queue->size);
    queue->current = 0;
    tile_id = 0;
    i = 0;
    while (i < tiles_y)
    {
        j = 0;
        while (j < tiles_x)
        {
            queue->tiles[tile_id].tile_id = tile_id;
            queue->tiles[tile_id].x = j * tile_size;
            queue->tiles[tile_id].y = i * tile_size;
            if (j == tiles_x - 1)
                queue->tiles[tile_id].width = width - (j * tile_size);
            else
                queue->tiles[tile_id].width = tile_size;
            if (i == tiles_y - 1)
                queue->tiles[tile_id].height = height - (i * tile_size);
            else
                queue->tiles[tile_id].height = tile_size;
            tile_id++;
            j++;
        }
        i++;
    }
}

bool    queue_next_job(t_queue *queue, t_tile *tile)
{
    pthread_mutex_lock(&queue->lock);
    if (queue->current < queue->size)
        queue->current++;
    pthread_mutex_unlock(&queue->lock);
    if (queue->current <= queue->size)
    {
        *tile = queue->tiles[queue->current - 1];
        return (true);
    }
    return (false);
}

void    destroy_queue(t_queue *queue)
{
    free(queue->tiles);
    pthread_mutex_destroy(&queue->lock);
}
