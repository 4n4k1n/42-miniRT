/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   broadcast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 00:00:00 by claude            #+#    #+#             */
/*   Updated: 2025/11/12 17:04:25 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Perform initial handshake and setup for a connected worker context.
 * Registers the worker with the master, sends default settings and the scene
 * file, then waits for the MSG_WORKER_READY header from the remote side.
 * @param context worker connection context
 * @param master master server state
 * @return 1 on success (worker ready), 0 on failure
 */
static int	worker_setup(t_worker_context *context, t_master *master)
{
	t_msg_header	header;
	t_settings		settings;

	printf("Worker connected from socket: %d\n", context->worker_socket);
	register_worker(master, context->worker_socket);
	init_worker_settings(&settings);
	send_settings(context->worker_socket, &settings);
	send_file(master->scene_file, context->worker_socket);
	header = recive_header(context->worker_socket);
	if (header.msg_type != MSG_WORKER_READY)
		return (0);
	printf("Worker %d ready, waiting for render signal...\n",
		context->worker_socket);
	return (1);
}

/**
 * Main per-worker thread loop: process jobs and wait for restart signals.
 * Continues until the master requests shutdown.
 * @param context worker connection context
 * @param master master server state
 */
static void	worker_main(t_worker_context *context, t_master *master)
{
	printf("Worker %d starting render\n", context->worker_socket);
	while (!master->shutdown)
	{
		worker_render_loop(context, master);
		worker_wait_for_restart(master, context->worker_socket);
	}
}

/**
 * Thread entry point for a worker handler.
 * Executes setup, waits for the global start_render flag and then runs the
 * main worker loop. Cleans up and notifies the worker on shutdown.
 * @param arg pointer to t_worker_context
 * @return NULL on thread exit
 */
void	*worker_thread_func(void *arg)
{
	t_worker_context	*context;
	t_master			*master;

	context = (t_worker_context *)arg;
	master = context->master;
	if (!worker_setup(context, master))
		return (handle_worker_disconnect(context, master));
	while (!master->start_render && !master->shutdown)
		usleep(100000);
	if (master->shutdown)
		return (handle_worker_disconnect(context, master));
	worker_main(context, master);
	unregister_worker(master, context->worker_socket);
	send_header(context->worker_socket, MSG_SHUTDOWN, 0);
	return (close(context->worker_socket), free(context), NULL);
}

/**
 * Main loop executed by a standalone worker client
 * after connecting to the master.
 * Receives top-level headers and
 * delegates handling to handle_msg until shutdown.
 * Tracks number of rendered tiles and prints final summary before exit.
 * @param master_socket connected socket to master
 * @param data local rendering context
 * @return 0 on clean exit
 */
static int	worker_main_loop(int master_socket, t_data *data)
{
	uint32_t		tiles_rendered;
	t_msg_header	header;

	tiles_rendered = 0;
	while (true)
	{
		header = recive_header(master_socket);
		if (handle_msg(master_socket, data, &header, &tiles_rendered))
			break ;
	}
	printf("Worker shutting down. Total tiles rendered: %d\n", tiles_rendered);
	return (0);
}

/**
 * Client-side entry point for running a worker that connects to a master.
 * Connects, receives settings and scene, signals readiness and enters the
 * main worker loop. Cleans up threads and scene data on exit.
 * @param master_ip master IPv4 address string
 * @param port TCP port to connect to
 * @return 0 on success, non-zero on failure
 */
int	run_worker(char *master_ip, uint32_t port)
{
	int		master_socket;
	t_data	data;

	master_socket = connect_to_master(master_ip, port);
	if (master_socket < 0)
		return (1);
	data.settings = recive_settings(master_socket);
	if (setup_scene_file(master_socket, &data))
	{
		close(master_socket);
		return (1);
	}
	send_header(master_socket, MSG_WORKER_READY, 0);
	printf("ready to render\n");
	worker_main_loop(master_socket, &data);
	cleanup_data(&data);
	close(master_socket);
	free_scene(&data);
	return (0);
}
