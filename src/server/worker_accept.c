/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worker_accept.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 13:00:00 by anakin            #+#    #+#             */
/*   Updated: 2025/11/12 16:48:23 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Allocate and start a detached thread to handle a newly connected worker.
 * On allocation failure the socket is closed and an error is reported.
 * The new worker context holds the master pointer and the accepted socket.
 * @param master pointer to master server state
 * @param worker_socket accepted client socket descriptor
 * @return NULL on success, or NULL after performing error handling on failure
 */
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

/**
 * Thread function that accepts incoming worker TCP connections.
 * Loops until master->shutdown is set. For each accepted connection it
 * enforces a maximum worker limit and spawns a handler thread.
 * @param arg pointer to t_master structure
 * @return NULL when the accept loop exits
 */
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
