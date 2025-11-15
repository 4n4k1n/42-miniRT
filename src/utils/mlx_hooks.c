/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_hooks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 23:21:04 by anakin            #+#    #+#             */
/*   Updated: 2025/11/15 09:36:31 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Handles WASD lateral movement by moving the camera along the forward/right
 * basis computed from the camera orientation.
 * @param data global scene data containing camera
 * @param k key event data (key identifies W/A/S/D)
 * @param dist movement scale factor
 */
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

/**
 * Adjusts camera pitch/yaw for arrow-key rotations.
 * @param data global scene data containing camera
 * @param keydata key event describing which arrow was pressed
 */
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

/**
 * Handles special non-movement keys: close, AA toggle, light toggle and
 * viewport scale adjustments. Returns 1 when the caller should stop further
 * processing (key consumed) otherwise 0.
 * @param data global scene data
 * @param k key event data
 * @param dist pointer to movement scale (may be modified)
 * @return 1 if key was handled and no further processing required, 0 otherwise
 */
static int	handle_special_keys(t_data *data, mlx_key_data_t k, float *dist)
{
	if (k.key == MLX_KEY_ESCAPE)
		return (mlx_close_window(data->mlx), 1);
	else if (k.key == MLX_KEY_LEFT_BRACKET)
		return (*dist *= 0.8, 1);
	else if (k.key == MLX_KEY_RIGHT_BRACKET)
		return (*dist *= 1.2, 1);
	else if (k.key == MLX_KEY_R)
	{
		data->settings.aa_state = !data->settings.aa_state;
		return (0);
	}
	else if (k.key == MLX_KEY_L)
		data->settings.light_state = !data->settings.light_state;
	return (0);
}

/**
 * Dispatches movement and rotation key handling to the appropriate helper.
 * @param data global scene data
 * @param k key event data
 * @param dist movement scale factor
 */
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

/**
 * Global key event hook installed into MLX.
 * Responds to press/repeat events for movement, rotation and special keys.
 * After handling updates camera matrices and triggers a re-render or
 * broadcasts an update when a master connection exists in BONUS mode.
 * @param keydata MLX key event structure
 * @param param pointer to t_data scene context
 */
void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_data			*data;
	static float	move_distance = 1.0f;

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
		if (handle_special_keys(data, keydata, &move_distance))
			return ;
		handle_movement(data, keydata, move_distance);
		update_camera(data);
		if (data->master && !data->is_local)
			broadcast_update(data->master, keydata.key);
		else
			render(data);
	}
}
