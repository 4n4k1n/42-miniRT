/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worker_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 13:00:00 by anakin            #+#    #+#             */
/*   Updated: 2025/11/12 16:50:54 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Initialize a t_settings structure with default worker-side values.
 * This mirrors the defaults used by the master when assigning work.
 * @param settings pointer to settings to initialize
 */
void	init_worker_settings(t_settings *settings)
{
	settings->aa_state = ANTI_ALIASING;
	settings->depth = MAX_DEPTH;
	settings->light_state = false;
	settings->max_samples = AA_MIN_SAMPLES * 8;
	settings->min_samples = AA_MIN_SAMPLES;
	settings->scale = SCALE;
	settings->shadow_samples = SHADOW_SAMPLES;
	settings->use_bvh = USE_BVH;
}

/**
 * Handle a worker disconnect by unregistering and cleaning up the context.
 * Closes the socket, removes the worker from master state and frees the
 * worker context memory.
 * @param context pointer to worker context to remove
 * @param master pointer to master state for unregistering
 * @return NULL (convenience for thread-return usage)
 */
void	*handle_worker_disconnect(t_worker_context *context, t_master *master)
{
	unregister_worker(master, context->worker_socket);
	close(context->worker_socket);
	free(context);
	return (NULL);
}

/**
 * Process a single tile job from the master's queue for this worker.
 * Requests the next job, sends the assignment to the worker socket,
 * receives the rendered result and copies it into the master's framebuffer
 * while holding the image mutex.
 * @param context worker context containing socket info
 * @param master master state containing job queue and framebuffer
 */
void	process_tile_job(t_worker_context *context, t_master *master)
{
	t_tile		tile;
	t_tile		result;
	uint32_t	*pixels;

	if (!queue_next_job(master->queue, &tile))
		return ;
	send_tile_assignment(context->worker_socket, &tile);
	recive_tile_result(context->worker_socket, &result, &pixels);
	pthread_mutex_lock(&master->img_lock);
	copy_tile_to_framebuffer(master->img, &result, pixels);
	pthread_mutex_unlock(&master->img_lock);
	free(pixels);
	printf("Tile %d completed (%zu/%zu)\n", result.tile_id,
		master->queue->current, master->queue->size);
}

/**
 * Loop that continuously processes tile jobs until master shutdown.
 * Prints status when the worker finishes and waits for further updates.
 * @param context worker context for this thread
 * @param master master server state
 */
void	worker_render_loop(t_worker_context *context, t_master *master)
{
	while (!master->shutdown)
		process_tile_job(context, master);
	printf("Worker %d finished rendering, waiting for updates...\n",
		context->worker_socket);
}

/**
 * Block until the master signals a render restart or shutdown.
 * Polls the master's restart_lock and sleeps in short intervals to avoid
 * busy-waiting. Returns when restart_render is set or shutdown is requested.
 * @param master master server state
 * @param worker_socket socket id for logging context
 */
void	worker_wait_for_restart(t_master *master, int worker_socket)
{
	while (!master->shutdown)
	{
		pthread_mutex_lock(&master->restart_lock);
		if (master->restart_render)
		{
			pthread_mutex_unlock(&master->restart_lock);
			printf("Worker %d restarting render...\n", worker_socket);
			break ;
		}
		pthread_mutex_unlock(&master->restart_lock);
		usleep(100000);
	}
}
