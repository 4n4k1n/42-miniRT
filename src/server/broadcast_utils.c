/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   boradcast_update.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 12:54:39 by anakin            #+#    #+#             */
/*   Updated: 2025/11/15 12:55:03 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static t_updated_scene_type	get_movement_update(uint32_t key)
{
	if (key == MLX_KEY_W)
		return (MOVE_FORWARD);
	else if (key == MLX_KEY_S)
		return (MOVE_BACKWARD);
	else if (key == MLX_KEY_A)
		return (MOVE_LEFT);
	else if (key == MLX_KEY_D)
		return (MOVE_RIGHT);
	else if (key == MLX_KEY_SPACE)
		return (MOVE_UP);
	else if (key == MLX_KEY_LEFT_SHIFT)
		return (MOVE_DOWN);
	return (UPDATE_RENDERING);
}

static t_updated_scene_type	get_look_update(uint32_t key)
{
	if (key == MLX_KEY_UP)
		return (LOOK_UP);
	else if (key == MLX_KEY_DOWN)
		return (LOOK_DOWN);
	else if (key == MLX_KEY_LEFT)
		return (LOOK_LEFT);
	else if (key == MLX_KEY_RIGHT)
		return (LOOK_RIGHT);
	return (UPDATE_RENDERING);
}

static t_updated_scene_type	get_setting_update(uint32_t key)
{
	if (key == MLX_KEY_R)
		return (UPDATE_RENDERING);
	else if (key == MLX_KEY_L)
		return (UPDATE_LIGHT);
	else if (key == MLX_KEY_LEFT_BRACKET)
		return (DIST_DECREASE);
	else if (key == MLX_KEY_RIGHT_BRACKET)
		return (DIST_INCREASE);
	return (UPDATE_RENDERING);
}

t_updated_scene_type	get_update_value(uint32_t key)
{
	t_updated_scene_type	result;

	result = get_movement_update(key);
	if (result != UPDATE_RENDERING)
		return (result);
	result = get_look_update(key);
	if (result != UPDATE_RENDERING)
		return (result);
	return (get_setting_update(key));
}
