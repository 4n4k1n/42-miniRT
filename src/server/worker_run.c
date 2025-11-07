/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worker_run.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 13:00:00 by anakin            #+#    #+#             */
/*   Updated: 2025/10/30 13:00:00 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

int	connect_to_master(char *master_ip, uint32_t port)
{
	int					master_socket;
	struct sockaddr_in	master_addr;

	master_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (master_socket < 0)
		return (ft_error("socket", 1));
	master_addr.sin_family = AF_INET;
	master_addr.sin_port = htons(port);
	inet_pton(AF_INET, master_ip, &master_addr.sin_addr);
	printf("Connecting to master %s:%d\n", master_ip, (int)port);
	if (connect(master_socket, (struct sockaddr *)&master_addr,
			sizeof(master_addr)) < 0)
	{
		close(master_socket);
		return (ft_error("connect", 1));
	}
	printf("Connected");
	return (master_socket);
}

int	setup_scene_file(int master_socket, t_data *data)
{
	char	*scene_content;
	int		scene_file_fd;

	scene_content = recive_scene_file(master_socket);
	if (!scene_content)
	{
		printf("failed to recive scene\n");
		return (1);
	}
	scene_file_fd = open("scene.rt", O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (scene_file_fd < 0)
	{
		free(scene_content);
		return (ft_error("open", 1));
	}
	write(scene_file_fd, scene_content, ft_strlen(scene_content));
	close(scene_file_fd);
	free(scene_content);
	parse_scene("scene.rt", data);
	if (data->settings.use_bvh && data->objects && data->objects->size > 0)
		data->bvh_root = build_bvh(data->objects);
	data->camera.samples_per_pixel = data->settings.max_samples;
	init_camera(data);
	init_threads_worker(data);
	return (0);
}

void	handle_camera_update(t_data *data, t_camera_update *cam_update)
{
	data->camera.cords.x = cam_update->x;
	data->camera.cords.y = cam_update->y;
	data->camera.cords.z = cam_update->z;
	data->camera.pitch = cam_update->pitch;
	data->camera.yaw = cam_update->yaw;
	data->settings.aa_state = cam_update->aa_state;
	data->settings.light_state = cam_update->light_state;
	update_camera(data);
	printf("Received camera update from master, ready for new render...\n");
}

int	process_tile_render(int master_socket, t_data *data,
	uint32_t *tiles_rendered)
{
	t_tile		tile;
	t_tile		result;
	uint32_t	*pixels;

	tile = recive_tile_assignment(master_socket);
	printf("Rendering tile %d (%dx%d at %d,%d)...\n", tile.tile_id,
		tile.width, tile.height, tile.x, tile.y);
	pixels = render_tile(data, &tile);
	if (!pixels)
	{
		printf("Failed to render tile %d\n", tile.tile_id);
		return (1);
	}
	result.height = tile.height;
	result.tile_id = tile.tile_id;
	result.width = tile.width;
	result.x = tile.x;
	result.y = tile.y;
	send_tile_result(master_socket, &result, pixels);
	free(pixels);
	(*tiles_rendered)++;
	printf("Tile %d completed (total: %d)\n", tile.tile_id, *tiles_rendered);
	return (0);
}

int	handle_msg(int master_socket, t_data *data,
	t_msg_header *header, uint32_t *tiles_rendered)
{
	t_camera_update	cam_update;

	if (header->msg_type == MSG_SHUTDOWN)
	{
		printf("recived shutdown\n");
		return (1);
	}
	if (header->msg_type == MSG_UPDATE)
	{
		recv_all(master_socket, &cam_update, sizeof(t_camera_update));
		handle_camera_update(data, &cam_update);
		return (0);
	}
	if (header->msg_type != MSG_RENDER_TILE)
	{
		printf("unexpected message type: %d\n", header->msg_type);
		return (1);
	}
	if (process_tile_render(master_socket, data, tiles_rendered))
		return (1);
	return (0);
}
