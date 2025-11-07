/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worker_accept.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 13:00:00 by anakin            #+#    #+#             */
/*   Updated: 2025/10/30 13:00:00 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static void	*handle_new_worker(t_master *master, int worker_socket)
{
	t_worker_context	*context;
	pthread_t			thread;

	context = malloc(sizeof(t_worker_context));
	if (!context)
		return (close(worker_socket), ft_error("malloc", 1), NULL);
	context->master = master;
	context->worker_socket = worker_socket;
	pthread_create(&thread, NULL, worker_thread_func, context);
	pthread_detach(thread);
	master->num_worker++;
	return (NULL);
}

void	*accept_worker_threads(void *arg)
{
	t_master	*master;
	int			worker_socket;

	master = (t_master *)arg;
	while (!master->shutdown)
	{
		worker_socket = accept(master->socket_fd, NULL, NULL);
		if (worker_socket < 0)
			continue ;
		if (master->num_worker >= MAX_WORKER)
		{
			close(worker_socket);
			continue ;
		}
		if (handle_new_worker(master, worker_socket))
			return (NULL);
	}
	return (NULL);
}
