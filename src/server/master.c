/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   master.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 11:41:09 by anakin            #+#    #+#             */
/*   Updated: 2025/11/12 17:05:41 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Print master server startup statistics and install the MLX key hook.
 * Displays scene info, resolution, tile counts and both local/public IPs.
 * Also wires the data.master pointer and prepares internal state used later.
 * @param r_master runtime master wrapper containing master + data
 * @param scene_file file path of the scene being served
 */
static void	print_master_stats(t_run_master *r_master, char *scene_file)
{
	printf("\n=== Master Server Ready ===\n");
	printf("Scene: %s\n", scene_file);
	printf("Resolution: %dx%d\n", (int)r_master->data.defines.width,
		(int)r_master->data.defines.height);
	printf("Tile size: %d\n", TILE_SIZE);
	printf("Total tiles: %zu\n\n", r_master->master.queue->size);
	printf("Local IP:  %s\n", get_ip_address());
	printf("Public IP: %s\n\n", get_public_ip());
	printf("Waiting for workers to connect...\n");
	printf("Press ENTER to start rendering\n\n");
	r_master->data.master = &r_master->master;
	mlx_key_hook(r_master->master.mlx, key_hook, &r_master->data);
	r_master->prev_workers = 0;
}

/**
 * Wait loop that prints worker connection changes and detects user ENTER to
 * start rendering.
 * Uses select on stdin with a short timeout to periodically refresh counts.
 * @param r_master runtime master wrapper containing master + data
 */
static void	connect_workers(t_run_master *r_master)
{
	while (1)
	{
		if (r_master->master.num_worker != r_master->prev_workers)
		{
			printf("Workers connected: %d\n", r_master->master.num_worker);
			r_master->prev_workers = r_master->master.num_worker;
		}
		FD_ZERO(&r_master->readfds);
		FD_SET(STDIN_FILENO, &r_master->readfds);
		r_master->tv.tv_sec = 0;
		r_master->tv.tv_usec = 100000;
		r_master->ret = select(STDIN_FILENO + 1, &r_master->readfds, NULL, NULL,
				&r_master->tv);
		if (r_master->ret > 0 && FD_ISSET(STDIN_FILENO, &r_master->readfds))
		{
			if (fgets(r_master->buffer, sizeof(r_master->buffer), stdin) != NULL
				&& r_master->buffer[0] == '\n')
				break ;
		}
	}
}

/**
 * Start the master server: setup networking, accept workers and begin render.
 * This initializes master state via setup_master, waits for operators to start
 * (ENTER), notifies workers to begin and then enters the MLX event loop.
 * On exit performs cleanup of threads, queue, mutexes and scene resources.
 * @param scene_file path to the scene file to serve
 * @param port TCP port to listen on for worker connections
 * @return 0 on success, non-zero on failure
 */
int	run_master(char *scene_file, uint32_t port)
{
	t_run_master	r_master;

	if (setup_master(&r_master.master, &r_master.data, scene_file, port) != 0)
		return (1);
	print_master_stats(&r_master, scene_file);
	connect_workers(&r_master);
	printf("Workers: %d\n", r_master.master.num_worker);
	printf("Tiles per worker: ~%zu\n\n", r_master.master.queue->size
		/ (r_master.master.num_worker));
	r_master.master.start_render = true;
	mlx_loop(r_master.master.mlx);
	r_master.master.shutdown = true;
	pthread_join(r_master.master.accept_thread, NULL);
	destroy_queue(r_master.master.queue);
	free(r_master.master.queue);
	pthread_mutex_destroy(&r_master.master.img_lock);
	pthread_mutex_destroy(&r_master.master.workers_lock);
	pthread_mutex_destroy(&r_master.master.restart_lock);
	mlx_terminate(r_master.master.mlx);
	free_scene(&r_master.data);
	return (0);
}
