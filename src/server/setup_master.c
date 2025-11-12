/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_master.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 10:01:22 by anakin            #+#    #+#             */
/*   Updated: 2025/11/12 17:06:37 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static bool	setup1(t_master *master, char *scene_file, t_data *d,
	t_defines defines)
{
	ft_memset(master, 0, sizeof(t_master));
	master->scene_file = scene_file;
	master->start_render = false;
	master->restart_render = false;
	master->data = d;
	if (parse_scene(scene_file, d))
		return (ft_error("parse_scene", 1));
	master->queue = malloc(sizeof(t_queue));
	if (!master->queue)
		return (free_scene(d), ft_error("malloc", 1));
	init_queue(master->queue, defines.width, defines.height, TILE_SIZE);
	master->mlx = mlx_init(defines.width, defines.height, "miniRT", false);
	if (!master->mlx)
		return (free(master->queue), free_scene(d), ft_error("mlx_init", 1));
	master->img = mlx_new_image(master->mlx, defines.width, defines.height);
	if (!master->img)
	{
		return (mlx_terminate(master->mlx), free(master->queue),
			free_scene(d), ft_error("mlx_new_image", 1));
	}
	mlx_image_to_window(master->mlx, master->img, 0, 0);
	pthread_mutex_init(&master->img_lock, NULL);
	pthread_mutex_init(&master->workers_lock, NULL);
	pthread_mutex_init(&master->restart_lock, NULL);
	return (0);
}

/**
 * Set up the master server state and start the accept thread.
 * Initializes defines via translate_settings (done by caller), parses the
 * scene, builds the tile queue, initializes MLX and mutexes, sets worker
 * socket slots to -1 and creates the listening socket. Finally spawns the
 * accept thread to handle incoming worker connections.
 * On any failure the function performs appropriate cleanup before returning.
 * @param master pointer to master state to populate
 * @param data pointer to scene/runtime data used by master
 * @param scene_file path to the scene file
 * @param port TCP port to listen on
 * @return 0 on success, 1 on error
 */
int	setup_master(t_master *master, t_data *data, char *scene_file,
		uint32_t port)
{
	int	i;

	translate_settings(data);
	if (setup1(master, scene_file, data, data->defines) == 1)
		return (1);
	i = 0;
	while (i < MAX_WORKER)
	{
		master->worker_sockets[i] = -1;
		i++;
	}
	master->socket_fd = setup_listen_socket(port);
	if (master->socket_fd < 0)
	{
		pthread_mutex_destroy(&master->img_lock);
		pthread_mutex_destroy(&master->workers_lock);
		pthread_mutex_destroy(&master->restart_lock);
		mlx_terminate(master->mlx);
		free(master->queue);
		free_scene(data);
		return (1);
	}
	pthread_create(&master->accept_thread, NULL, accept_worker_threads, master);
	return (0);
}
