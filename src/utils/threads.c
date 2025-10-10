/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apregitz <apregitz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 14:10:56 by apregitz          #+#    #+#             */
/*   Updated: 2025/10/10 18:21:33 by apregitz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "mini_rt.h"

int	change_thread_state(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->threads_amount)
	{
		pthread_mutex_lock(&data->threads[i].active_mutex);
		data->threads[i].active = !data->threads[i].active;
		pthread_mutex_unlock(&data->threads[i].active_mutex);
		i++;
	}
	return (0);
}

int	ft_wait(t_thread *thread)
{
	bool	active;
	bool	shutdown;

	active = 0;
	shutdown = 0;
	while (!active && !shutdown)
	{
		pthread_mutex_lock(&thread->active_mutex);
		active = thread->active;
		shutdown = thread->shutdown;
		pthread_mutex_unlock(&thread->active_mutex);
		usleep(500);
	}
	return (shutdown);
}

void	*thread_job(void *arg)
{
	t_thread	*thread;
	int			i;
	int			j;
	uint32_t	color;

	thread = (t_thread *)arg;
	while (true)
	{
		if (ft_wait(thread))
			break ;
		i = thread->id;
		while (i < HEIGHT)
		{
			j = 0;
			while (j < WIDTH)
			{
				if (ANTI_ALIASING)
					color = monte_carlo_aa(thread->data, i, j);
				else
					color = without_aa(thread->data, i, j);
				pthread_mutex_lock(&thread->data->pixel_mutex);
				mlx_put_pixel(thread->data->img, j, i, color);
				pthread_mutex_unlock(&thread->data->pixel_mutex);
				j++;
			}
			i += thread->data->threads_amount;
		}
		pthread_mutex_lock(&thread->active_mutex);
		thread->active = !thread->active;
		pthread_mutex_unlock(&thread->active_mutex);
		pthread_mutex_lock(&thread->data->threads_done_mutex);
		thread->data->threads_done++;
		pthread_mutex_unlock(&thread->data->threads_done_mutex);
	}
	return (NULL);
}

int	init_threads(t_data *data)
{
	int	i;
	data->threads_amount = sysconf(_SC_NPROCESSORS_ONLN);
	data->threads = ft_calloc(data->threads_amount, sizeof(t_thread));
	if (!data->threads)
		return (1);
	i = 0;
	while (i < data->threads_amount)
	{
		pthread_mutex_init(&data->threads[i].active_mutex, NULL);
		i++;
	}
	pthread_mutex_init(&data->threads_done_mutex, NULL);
	pthread_mutex_init(&data->pixel_mutex, NULL);
	i = 0;
	while (i < data->threads_amount)
	{
		data->threads[i].data = data;
		data->threads[i].id = i;
		data->threads[i].active = false;
		data->threads[i].shutdown = false;
		if (pthread_create(&data->threads[i].thread, NULL,
				thread_job, &data->threads[i]) != 0)
		{
			while (i-- >= 0)
				pthread_join(data->threads[i].thread, NULL);
			return (1);
		}
		i++;
	}
	return (0);
}

int	render_with_mt(t_data *data)
{
	int	checked;

	checked = 0;
	pthread_mutex_lock(&data->threads_done_mutex);
	data->threads_done = 0;
	pthread_mutex_unlock(&data->threads_done_mutex);
	change_thread_state(data);
	while (1)
	{
		pthread_mutex_lock(&data->threads_done_mutex);
		checked = data->threads_done;
		pthread_mutex_unlock(&data->threads_done_mutex);
		if (checked == data->threads_amount)
			break ;
		usleep(500);
	}
	pthread_mutex_lock(&data->threads_done_mutex);
	data->threads_done = 0;
	pthread_mutex_unlock(&data->threads_done_mutex);
	return (0);
}

void	cleanup_data(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->threads_amount)
	{
		pthread_mutex_lock(&data->threads[i].active_mutex);
		data->threads[i].shutdown = true;
		data->threads[i].active = true;
		pthread_mutex_unlock(&data->threads[i].active_mutex);
		i++;
	}
	i = 0;
	while (i < data->threads_amount)
	{
		pthread_join(data->threads[i].thread, NULL);
		i++;
	}
	i = 0;
	while (i < data->threads_amount)
	{
		pthread_mutex_destroy(&data->threads[i].active_mutex);
		i++;
	}
	pthread_mutex_destroy(&data->threads_done_mutex);
	pthread_mutex_destroy(&data->pixel_mutex);
	free(data->threads);
}
