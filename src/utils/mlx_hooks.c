/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_hooks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apregitz <apregitz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 23:21:04 by anakin            #+#    #+#             */
/*   Updated: 2025/10/29 14:31:53 by apregitz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static void	handle_wasd_movement(t_data *data, mlx_key_data_t k, float dist)
{
	t_vec3	forward;
	t_vec3	right;
	t_vec3	up;
	t_vec3	move_vec;

	get_camera_vectors(data, &forward, &right, &up);
	if (k.key == MLX_KEY_W)
		move_vec = vec3_multiply(forward, -0.2 * dist);
	else if (k.key == MLX_KEY_S)
		move_vec = vec3_multiply(forward, 0.2 * dist);
	else if (k.key == MLX_KEY_A)
		move_vec = vec3_multiply(right, -0.2 * dist);
	else
		move_vec = vec3_multiply(right, 0.2 * dist);
	data->camera.cords = vec3_add(data->camera.cords, move_vec);
}

static void	handle_rotation(t_data *data, mlx_key_data_t keydata)
{
	if (keydata.key == MLX_KEY_UP)
		data->camera.pitch -= 0.1;
	else if (keydata.key == MLX_KEY_DOWN)
		data->camera.pitch += 0.1;
	else if (keydata.key == MLX_KEY_LEFT)
		data->camera.yaw += 0.1;
	else if (keydata.key == MLX_KEY_RIGHT)
		data->camera.yaw -= 0.1;
}

static int	handle_special_keys(t_data *data, mlx_key_data_t k, float *dist)
{
	if (k.key == MLX_KEY_ESCAPE)
		return (mlx_close_window(data->mlx), 1);
	else if (k.key == MLX_KEY_LEFT_BRACKET)
		return (*dist *= 0.8, 1);
	else if (k.key == MLX_KEY_RIGHT_BRACKET)
		return (*dist *= 1.2, 1);
	else if (k.key == MLX_KEY_R)
		return (data->settings.aa_state = !data->settings.aa_state,
			render(data), 1);
	else if (k.key == MLX_KEY_L)
		data->settings.light_state = !data->settings.light_state;
	return (0);
}

static void	handle_movement(t_data *data, mlx_key_data_t k, float dist)
{
	if (k.key == MLX_KEY_W || k.key == MLX_KEY_S
		|| k.key == MLX_KEY_A || k.key == MLX_KEY_D)
		handle_wasd_movement(data, k, dist);
	else if (k.key == MLX_KEY_SPACE)
		data->camera.cords.y += 0.2 * dist;
	else if (k.key == MLX_KEY_LEFT_SHIFT)
		data->camera.cords.y -= 0.2 * dist;
	else if (k.key == MLX_KEY_UP || k.key == MLX_KEY_DOWN
		|| k.key == MLX_KEY_LEFT || k.key == MLX_KEY_RIGHT)
		handle_rotation(data, k);
}

void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_data			*data;
	static float	move_distace = 1.0;

	data = (t_data *)param;
	if ((keydata.action == MLX_PRESS || keydata.action == MLX_REPEAT)
		&& (keydata.key == MLX_KEY_W || keydata.key == MLX_KEY_S
			|| keydata.key == MLX_KEY_A || keydata.key == MLX_KEY_D
			|| keydata.key == MLX_KEY_SPACE || keydata.key == MLX_KEY_LEFT_SHIFT
			|| keydata.key == MLX_KEY_UP || keydata.key == MLX_KEY_DOWN
			|| keydata.key == MLX_KEY_LEFT || keydata.key == MLX_KEY_RIGHT
			|| keydata.key == MLX_KEY_ESCAPE || keydata.key == MLX_KEY_R
			|| keydata.key == MLX_KEY_L || keydata.key == MLX_KEY_LEFT_BRACKET
			|| keydata.key == MLX_KEY_RIGHT_BRACKET))
	{
		if (handle_special_keys(data, keydata, &move_distace))
			return ;
		handle_movement(data, keydata, move_distace);
		update_camera(data);
		if (data->master && BONUS)
			broadcast_update(data->master, 1);
		else
			render(data);
	}
}
