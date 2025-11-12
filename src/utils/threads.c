/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 14:10:56 by apregitz          #+#    #+#             */
/*   Updated: 2025/11/12 15:31:16 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Renders a single image row for the thread and writes pixels to the MLX image.
 * Iterates over columns, chooses sampling path depending on AA setting and
 * writes the computed color into the screen image via mlx_put_pixel.
 * @param thread pointer to thread context (contains data and id)
 * @param i      row index to render
 */
static void	thread_put_row(t_thread *thread, int i)
{
	uint32_t	color;
	int			j;

	j = 0;
	while (j < thread->data->defines.width)
	{
		if (thread->data->settings.aa_state)
			color = monte_carlo_aa(thread->data, i, j);
		else
			color = without_aa(thread->data, i, j);
		mlx_put_pixel(thread->data->img, j, i, color);
		j++;
	}
}

/**
 * Worker entry function for threads that render full-image rows.
 * The thread waits for a start signal (ft_wait). When activated it renders
 * every Nth row starting at its id, toggles its active flag and increments the
 * global threads_done counter. The loop exits when ft_wait indicates shutdown.
 * @param arg pointer to t_thread context
 * @return NULL
 */
void	*thread_job(void *arg)
{
	t_thread	*thread;
	int			i;

	thread = (t_thread *)arg;
	while (true)
	{
		if (ft_wait(thread))
			break ;
		i = thread->id;
		while (i < thread->data->defines.height)
		{
			thread_put_row(thread, i);
			i += thread->data->threads_amount;
		}
		pthread_mutex_lock(&thread->active_mutex);
		thread->active = !thread->active;
		pthread_mutex_unlock(&thread->active_mutex);
		__atomic_add_fetch(&thread->data->threads_done, 1, __ATOMIC_SEQ_CST);
	}
	return (NULL);
}

// __attribute__((force_inline))

/**
 * Renders a single row of the assigned tile into the thread-local pixel buffer.
 * Uses the tile origin plus local column index to compute absolute pixel
 * coordinates for sampling,
 * and writes the sampled color into thread->data->pixels.
 * @param thread pointer to worker thread context containing tile info
 * @param i      row index within the tile (0 .. tile->height-1)
 */
static void	thread_render_tile_row(t_thread *thread, int i)
{
	uint32_t	j;
	uint32_t	pixel_x;
	uint32_t	pixel_y;

	j = 0;
	while (j < thread->tile->width)
	{
		pixel_x = thread->tile->x + j;
		pixel_y = thread->tile->y + i;
		if (thread->data->settings.aa_state)
			thread->data->pixels[i * thread->tile->width
				+ j] = monte_carlo_aa(thread->data, pixel_y, pixel_x);
		else
			thread->data->pixels[i * thread->tile->width
				+ j] = without_aa(thread->data, pixel_y, pixel_x);
		j++;
	}
}

/**
 * Worker entry for tile-based rendering.
 * Waits for a tile assignment via ft_wait, renders rows of the tile in a
 * strided fashion based on threads_amount, toggles the active flag and
 * increments the global threads_done counter. Returns when ft_wait signals exit.
 * @param arg pointer to t_thread context
 * @return NULL
 */
void	*thread_job_worker(void *arg)
{
	t_thread	*thread;
	uint32_t	i;

	thread = (t_thread *)arg;
	while (true)
	{
		if (ft_wait(thread))
			break ;
		i = thread->id;
		while (i < thread->tile->height)
		{
			thread_render_tile_row(thread, i);
			i += thread->data->threads_amount;
		}
		pthread_mutex_lock(&thread->active_mutex);
		thread->active = !thread->active;
		pthread_mutex_unlock(&thread->active_mutex);
		__atomic_add_fetch(&thread->data->threads_done, 1, __ATOMIC_SEQ_CST);
	}
	return (NULL);
}
