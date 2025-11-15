/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   broadcast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 00:00:00 by nweber            #+#    #+#             */
/*   Updated: 2025/11/13 13:36:06 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Register a worker socket in the master's worker table.
 * Scans for the first available slot and stores the provided socket fd.
 * This function locks master->workers_lock to protect concurrent access.
 * If no slot is available the function returns without change.
 * @param master pointer to the master server state
 * @param socket_fd file descriptor of the worker socket to register
 */
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

/**
 * Unregister a worker socket from the master's worker table.
 * Finds the entry matching socket_fd and sets it to -1. Access to the table
 * is protected by master->workers_lock. If the socket is not found nothing
 * is changed.
 * @param master pointer to the master server state
 * @param socket_fd file descriptor of the worker socket to remove
 */
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

/**
 * Send a camera update to all currently registered workers.
 * Iterates over master->worker_sockets and for each valid socket sends a
 * MSG_UPDATE header followed by the t_camera_update payload. The caller
 * must hold the workers_lock (this function assumes the lock is held by the
 * caller to avoid concurrent modifications to the socket table).
 * Logs each successful send to stdout.
 * @param master pointer to the master server state (socket table consulted)
 * @param cam_update pointer to the camera update payload to broadcast
 */
static void	send_broadcast(t_master *master, t_camera_update *cam_update)
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
/**
 * Broadcast a camera/state update to all workers and restart the render.
 * Prepares a t_camera_update from master's current scene camera and settings,
 * then sends it to every registered worker. The function resets the job queue,
 * toggles master->restart_render to notify worker threads and sleeps briefly
 * to give workers time to receive the update. Uses workers_lock when iterating
 * over sockets and restart_lock for atomic toggling of restart_render.
 * The update_value parameter is currently unused (kept for API compatibility).
 * @param master pointer to the master server state
 * @param update_value unused numeric tag for the update (ignored)
 */
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
