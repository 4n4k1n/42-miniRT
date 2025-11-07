/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apregitz <apregitz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 14:10:56 by apregitz          #+#    #+#             */
/*   Updated: 2025/10/29 16:22:36 by apregitz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static void	thread_put_row(t_thread *thread, int i)
{
	uint32_t	color;
	int			j;

	j = 0;
	while (j < WIDTH)
	{
		if (thread->data->settings.aa_state)
			color = monte_carlo_aa(thread->data, i, j);
		else
			color = without_aa(thread->data, i, j);
		mlx_put_pixel(thread->data->img, j, i, color);
		j++;
	}
}

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
		while (i < HEIGHT)
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
