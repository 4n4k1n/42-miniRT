/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apregitz <apregitz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 16:04:53 by apregitz          #+#    #+#             */
/*   Updated: 2025/10/29 16:05:44 by apregitz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

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
