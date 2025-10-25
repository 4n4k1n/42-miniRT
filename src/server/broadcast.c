/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   broadcast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claude <claude@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 00:00:00 by claude            #+#    #+#             */
/*   Updated: 2025/10/24 00:00:00 by claude           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

void	register_worker(t_master *master, int socket_fd)
{
	int	i;

	pthread_mutex_lock(&master->workers_lock);
	i = 0;
	while (i < MAX_WORKER)
	{
		if (master->worker_sockets[i] == -1)
		{
			master->worker_sockets[i] = socket_fd;
			break ;
		}
		i++;
	}
	pthread_mutex_unlock(&master->workers_lock);
}

void	unregister_worker(t_master *master, int socket_fd)
{
	int	i;

	pthread_mutex_lock(&master->workers_lock);
	i = 0;
	while (i < MAX_WORKER)
	{
		if (master->worker_sockets[i] == socket_fd)
		{
			master->worker_sockets[i] = -1;
			break ;
		}
		i++;
	}
	pthread_mutex_unlock(&master->workers_lock);
}

void	broadcast_update(t_master *master, uint32_t update_value)
{
	int	i;

	pthread_mutex_lock(&master->workers_lock);
	i = 0;
	while (i < MAX_WORKER)
	{
		if (master->worker_sockets[i] != -1)
		{
			send_update(master->worker_sockets[i], update_value);
			printf("Sent update to worker socket %d\n", master->worker_sockets[i]);
		}
		i++;
	}
	pthread_mutex_unlock(&master->workers_lock);
}
