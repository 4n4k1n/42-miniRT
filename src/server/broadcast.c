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
	int				i;
	t_camera_update	cam_update;

	(void)update_value;
	printf("\n=== Broadcasting Update & Restarting Render ===\n");
	cam_update.x = master->data->camera.cords.x;
	cam_update.y = master->data->camera.cords.y;
	cam_update.z = master->data->camera.cords.z;
	cam_update.pitch = master->data->camera.pitch;
	cam_update.yaw = master->data->camera.yaw;
	cam_update.aa_state = master->data->settings.aa_state;
	cam_update.light_state = master->data->settings.light_state;
	pthread_mutex_lock(&master->workers_lock);
	i = 0;
	while (i < MAX_WORKER)
	{
		if (master->worker_sockets[i] != -1)
		{
			send_header(master->worker_sockets[i], MSG_UPDATE, sizeof(t_camera_update));
			send_all(master->worker_sockets[i], &cam_update, sizeof(t_camera_update));
			printf("Sent camera update to worker socket %d\n", master->worker_sockets[i]);
		}
		i++;
	}
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
