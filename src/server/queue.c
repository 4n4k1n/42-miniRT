/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 23:17:51 by anakin            #+#    #+#             */
/*   Updated: 2025/11/12 17:06:13 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static void	fill_tiles(t_queue_tmp *tmp)
{
	tmp->i = 0;
	while (tmp->i < tmp->tiles_y)
	{
		tmp->j = 0;
		while (tmp->j < tmp->tiles_x)
		{
			tmp->queue->tiles[tmp->tile_id].tile_id = tmp->tile_id;
			tmp->queue->tiles[tmp->tile_id].x = tmp->j * tmp->tile_size;
			tmp->queue->tiles[tmp->tile_id].y = tmp->i * tmp->tile_size;
			if (tmp->j == tmp->tiles_x - 1)
				tmp->queue->tiles[tmp->tile_id].width = tmp->width - (tmp->j
						* tmp->tile_size);
			else
				tmp->queue->tiles[tmp->tile_id].width = tmp->tile_size;
			if (tmp->i == tmp->tiles_y - 1)
				tmp->queue->tiles[tmp->tile_id].height = tmp->height - (tmp->i
						* tmp->tile_size);
			else
				tmp->queue->tiles[tmp->tile_id].height = tmp->tile_size;
			tmp->tile_id++;
			tmp->j++;
		}
		tmp->i++;
	}
}

/**
 * Initialize a tile queue for the given framebuffer dimensions and tile size.
 * Allocates the tile array, initializes the mutex and fills the queue with
 * tile descriptors covering the image area. The queue->current index is set
 * to zero and queue->size holds the total number of tiles.
 * @param queue pointer to queue to initialize (must be preallocated)
 * @param width framebuffer width in pixels
 * @param height framebuffer height in pixels
 * @param tile_size tile size in pixels (square tiles)
 */
void	init_queue(t_queue *queue, uint32_t width, uint32_t height,
		uint32_t tile_size)
{
	t_queue_tmp	tmp;

	tmp.height = height;
	tmp.width = width;
	tmp.tile_size = tile_size;
	tmp.queue = queue;
	pthread_mutex_init(&queue->lock, NULL);
	tmp.tiles_x = (width + tile_size - 1) / tile_size;
	tmp.tiles_y = (height + tile_size - 1) / tile_size;
	queue->size = tmp.tiles_x * tmp.tiles_y;
	queue->tiles = malloc(sizeof(t_tile) * queue->size);
	queue->current = 0;
	tmp.tile_id = 0;
	fill_tiles(&tmp);
}

/**
 * Atomically obtain the next available tile job from the queue.
 * If a job is available writes it into *tile, increments the queue current
 * index and returns true. Otherwise returns false.
 * @param queue pointer to job queue
 * @param tile out parameter to receive the next tile
 * @return true if a job was obtained, false if queue is exhausted
 */
bool	queue_next_job(t_queue *queue, t_tile *tile)
{
	bool	has_job;

	pthread_mutex_lock(&queue->lock);
	if (queue->current < queue->size)
	{
		*tile = queue->tiles[queue->current];
		queue->current++;
		has_job = true;
	}
	else
		has_job = false;
	pthread_mutex_unlock(&queue->lock);
	return (has_job);
}

/**
 * Reset the job queue so tiles can be repartitioned/restarted.
 * Sets current back to zero under the queue lock.
 * @param queue pointer to job queue
 */
void	reset_queue(t_queue *queue)
{
	pthread_mutex_lock(&queue->lock);
	queue->current = 0;
	pthread_mutex_unlock(&queue->lock);
}

/**
 * Destroy the queue: free tiles array and destroy associated mutex.
 * @param queue pointer to queue to destroy
 */
void	destroy_queue(t_queue *queue)
{
	free(queue->tiles);
	pthread_mutex_destroy(&queue->lock);
}
