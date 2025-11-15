/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   broadcast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 00:00:00 by claude            #+#    #+#             */
/*   Updated: 2025/11/15 12:57:20 by anakin           ###   ########.fr       */
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

static void	send_broadcast(t_master *master, t_update *update)
{
	int	i;

	i = 0;
	while (i < MAX_WORKER)
	{
		if (master->worker_sockets[i] != -1)
		{
			send_header(master->worker_sockets[i], MSG_UPDATE,
				sizeof(t_update));
			send_all(master->worker_sockets[i], update,
				sizeof(t_update));
			printf("Sent camera update to worker socket %d\n",
				master->worker_sockets[i]);
		}
		i++;
	}
}

void	broadcast_update(t_master *master, uint32_t update_value)
{
	t_update	update;

	printf("\n=== Broadcasting Update & Restarting Render ===\n");
	update.updated_varible = get_update_value(update_value);
	pthread_mutex_lock(&master->workers_lock);
	send_broadcast(master, &update);
	pthread_mutex_unlock(&master->workers_lock);
	reset_queue(master->queue);
	pthread_mutex_lock(&master->restart_lock);
	master->restart_render = true;
	pthread_mutex_unlock(&master->restart_lock);
	usleep(200000);
	pthread_mutex_lock(&master->restart_lock);
	master->restart_render = false;
	pthread_mutex_unlock(&master->restart_lock);
	printf("Queue reset: %zu tiles ready\n", master->queue->size);
}
