/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 16:04:53 by apregitz          #+#    #+#             */
/*   Updated: 2025/11/12 15:29:35 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Signals all worker threads to shut down, joins them and destroys their
 * synchronization primitives. Frees the threads array on completion.
 * This function sets each thread's shutdown flag and signals its condition
 * variable so a blocked thread will wake and exit. After joining all threads
 * it destroys the per-thread mutex/cond and frees the allocated array.
 * @param data shared application data containing threads information
 */
void	cleanup_data(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->threads_amount)
	{
		pthread_mutex_lock(&data->threads[i].active_mutex);
		data->threads[i].shutdown = true;
		data->threads[i].active = true;
		pthread_cond_signal(&data->threads[i].active_cond);
		pthread_mutex_unlock(&data->threads[i].active_mutex);
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
		pthread_cond_destroy(&data->threads[i].active_cond);
		i++;
	}
	free(data->threads);
}

/**
 * Toggles the active state for every worker thread and signals them so they
 * can start or resume work.
 * The function acquires each thread's active_mutex, flips the active flag,
 * signals the condition variable to wake the thread, then releases the mutex.
 * @param data shared application data containing thread contexts
 * @return always returns 0 (compatible with callers expecting an int)
 */
int	change_thread_state(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->threads_amount)
	{
		pthread_mutex_lock(&data->threads[i].active_mutex);
		data->threads[i].active = !data->threads[i].active;
		pthread_cond_signal(&data->threads[i].active_cond);
		pthread_mutex_unlock(&data->threads[i].active_mutex);
		i++;
	}
	return (0);
}

/**
 * Wait helper used by worker threads.
 * Blocks the calling thread on its condition variable until either the
 * active flag becomes true (work assigned) or the shutdown flag is set.
 * Returns the shutdown flag value so the caller can decide to exit.
 * @param thread pointer to the thread context
 * @return non-zero when shutdown was requested, zero otherwise
 */
int	ft_wait(t_thread *thread)
{
	bool	shutdown;

	pthread_mutex_lock(&thread->active_mutex);
	while (!thread->active && !thread->shutdown)
		pthread_cond_wait(&thread->active_cond, &thread->active_mutex);
	shutdown = thread->shutdown;
	pthread_mutex_unlock(&thread->active_mutex);
	return (shutdown);
}
