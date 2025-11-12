/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 15:56:27 by apregitz          #+#    #+#             */
/*   Updated: 2025/11/12 15:30:56 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Creates worker threads that run tile-based rendering (thread_job_worker).
 * Each thread context is initialized with data/id and flags. On pthread_create
 * failure the function joins already created threads and returns non-zero.
 * @param data shared application data containing thread array and count
 * @return 0 on success, non-zero on failure
 */
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

/**
 * Initializes thread pool for worker/tile-based rendering.
 * Allocates the thread contexts array sized to the number of online CPUs,
 * initializes per-thread mutex/cond variables and starts the worker threads.
 * @param data shared application data to populate
 * @return 0 on success, 1 on allocation or thread creation failure
 */
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

/**
 * Creates full-image rendering threads (thread_job).
 * Same semantics as create_worker_threads but spawns threads that render
 * entire rows instead of tiles. Joins created threads on failure.
 * @param data shared application data containing thread array and count
 * @return 0 on success, non-zero on failure
 */
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

/**
 * Initializes thread pool for full-image rendering.
 * Allocates per-thread contexts for the number of online processors,
 * initializes synchronization primitives and starts the rendering threads.
 * @param data shared application data to populate
 * @return 0 on success, 1 on allocation or thread creation failure
 */
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
