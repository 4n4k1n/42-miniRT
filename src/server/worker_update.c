/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worker_update.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 18:57:22 by anakin            #+#    #+#             */
/*   Updated: 2025/11/14 19:29:12 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static void	handle_camera_update_move(t_data *data, uint32_t update, float dist)
{
	t_vec3	forward;
	t_vec3	right;
	t_vec3	up;
	t_vec3	move_vec;

	get_camera_vectors(data, &forward, &right, &up);
	if (update == MOVE_FORWARD)
		move_vec = vec3_multiply(forward, -0.2 * dist);
	else if (update == MOVE_BACKWARD)
		move_vec = vec3_multiply(forward, 0.2 * dist);
	else if (update == MOVE_LEFT)
		move_vec = vec3_multiply(right, -0.2 * dist);
	else if (update == MOVE_RIGHT)
		move_vec = vec3_multiply(right, 0.2 * dist);
	data->camera.cords = vec3_add(data->camera.cords, move_vec);
	update_camera(data);
	printf("Received camera update from master, ready for new render...\n");
}

static void	handle_camera_update_rotation(t_data *data, uint32_t update)
{
	if (update == LOOK_UP)
		data->camera.pitch -= 0.1;
	else if (update == LOOK_DOWN)
		data->camera.pitch += 0.1;
	else if (update == LOOK_LEFT)
		data->camera.yaw += 0.1;
	else if (update == LOOK_RIGHT)
		data->camera.yaw -= 0.1;
}

static int	handle_camera_update_special(t_data *data, uint32_t update, float *dist)
{
	if (update == DIST_INCREASE)
		return (*dist *= 0.75, 1);
	else if (update == DIST_DECREASE)
		return (*dist *= 1.25, 1);
	else if (update == UPDATE_RENDERING)
	{
		data->settings.aa_state = !data->settings.aa_state;
		return (0);
	}
	else if (update== UPDATE_LIGHT)
		data->settings.light_state = !data->settings.light_state;
	return (0);
}

static void	handle_camera_update_movement(t_data *data, uint32_t update, float dist)
{
	if (update <= MOVE_RIGHT)
		handle_camera_update_move(data, update, dist);
	else if (update == MOVE_UP)
		data->camera.cords.y += 0.2 * dist;
	else if (update == MOVE_DOWN)
		data->camera.cords.y -= 0.2 * dist;
	else if (update == LOOK_UP || update == LOOK_DOWN
		|| update == LOOK_LEFT || update == LOOK_RIGHT)
		handle_camera_update_rotation(data, update);
}

void	handle_camera_update(t_data *data, uint32_t update)
{
	static float	move_distance = 1.0f;

	if (handle_camera_update_special(data, update, &move_distance))
		return ;
	handle_camera_update_movement(data, update, move_distance);
	update_camera(data);
}
