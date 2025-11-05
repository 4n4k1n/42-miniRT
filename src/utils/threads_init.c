/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apregitz <apregitz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 15:56:27 by apregitz          #+#    #+#             */
/*   Updated: 2025/10/29 15:58:28 by apregitz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static int	create_worker_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->threads_amount)
	{
		data->threads[i].data = data;
		data->threads[i].id = i;
		data->threads[i].active = false;
		data->threads[i].shutdown = false;
		if (pthread_create(&data->threads[i].thread, NULL, thread_job_worker,
				&data->threads[i]) != 0)
		{
			while (i-- >= 0)
				pthread_join(data->threads[i].thread, NULL);
			return (1);
		}
		i++;
	}
	return (0);
}

int	init_threads_worker(t_data *data)
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
		pthread_cond_init(&data->threads[i].active_cond, NULL);
		i++;
	}
	return (create_worker_threads(data));
}

static int	create_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->threads_amount)
	{
		data->threads[i].data = data;
		data->threads[i].id = i;
		data->threads[i].active = false;
		data->threads[i].shutdown = false;
		if (pthread_create(&data->threads[i].thread, NULL, thread_job,
				&data->threads[i]) != 0)
		{
			while (i-- >= 0)
				pthread_join(data->threads[i].thread, NULL);
			return (1);
		}
		i++;
	}
	return (0);
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
		pthread_cond_init(&data->threads[i].active_cond, NULL);
		i++;
	}
	return (create_threads(data));
}
