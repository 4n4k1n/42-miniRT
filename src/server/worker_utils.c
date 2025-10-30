/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worker_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 13:00:00 by anakin            #+#    #+#             */
/*   Updated: 2025/10/30 13:00:00 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

void	init_worker_settings(t_settings *settings)
{
	settings->aa_state = ANTI_ALIASING;
	settings->depth = MAX_DEPTH;
	settings->light_state = false;
	settings->max_samples = AA_MAX_SAMPLES;
	settings->min_samples = AA_MIN_SAMPLES;
	settings->scale = SCALE;
	settings->shadow_samples = SHADOW_SAMPLES;
}

void	*handle_worker_disconnect(t_worker_context *context, t_master *master)
{
	unregister_worker(master, context->worker_socket);
	close(context->worker_socket);
	free(context);
	return (NULL);
}

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

void	worker_render_loop(t_worker_context *context, t_master *master)
{
	while (!master->shutdown)
		process_tile_job(context, master);
	printf("Worker %d finished rendering, waiting for updates...\n",
		context->worker_socket);
}

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
