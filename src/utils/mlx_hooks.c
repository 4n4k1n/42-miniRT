/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_hooks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 23:21:04 by anakin            #+#    #+#             */
/*   Updated: 2025/10/01 23:35:45 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Key hook function to handle arrow key presses
 * Captures arrow key events and prints them to stdout
 */
void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_data	*data;
	t_vec3	forward, right, up;
	t_vec3	move_vec;

	data = (t_data *)param;
	if ((keydata.action == MLX_PRESS || keydata.action == MLX_REPEAT) && (keydata.key == MLX_KEY_W
		|| keydata.key == MLX_KEY_S || keydata.key == MLX_KEY_A
		|| keydata.key == MLX_KEY_D || keydata.key == MLX_KEY_SPACE || keydata.key == MLX_KEY_LEFT_SHIFT 
		|| keydata.key == MLX_KEY_UP || keydata.key == MLX_KEY_DOWN 
		|| keydata.key == MLX_KEY_LEFT || keydata.key == MLX_KEY_RIGHT || keydata.key == MLX_KEY_ESCAPE))
	{
		if (keydata.key == MLX_KEY_ESCAPE)
			mlx_close_window(data->mlx);
		else if (keydata.key == MLX_KEY_W || keydata.key == MLX_KEY_S || keydata.key == MLX_KEY_A || keydata.key == MLX_KEY_D)
		{
			get_camera_vectors(data, &forward, &right, &up);
			if (keydata.key == MLX_KEY_W)
			{
				move_vec = vec3_multiply_inline(&forward, -0.2);
				data->camera.cords = vec3_add_inline(&data->camera.cords, &move_vec);
			}
			else if (keydata.key == MLX_KEY_S)
			{
				move_vec = vec3_multiply_inline(&forward, 0.2);
				data->camera.cords = vec3_add_inline(&data->camera.cords, &move_vec);
			}
			else if (keydata.key == MLX_KEY_A)
			{
				move_vec = vec3_multiply_inline(&right, -0.2);
				data->camera.cords = vec3_add_inline(&data->camera.cords, &move_vec);
			}
			else if (keydata.key == MLX_KEY_D)
			{
				move_vec = vec3_multiply_inline(&right, 0.2);
				data->camera.cords = vec3_add_inline(&data->camera.cords, &move_vec);
			}
		}
		else if (keydata.key == MLX_KEY_SPACE)
			data->camera.cords.y += 0.2;
		else if (keydata.key == MLX_KEY_LEFT_SHIFT)
			data->camera.cords.y -= 0.2;
		else if (keydata.key == MLX_KEY_UP)
			data->camera.pitch -= 0.1;
		else if (keydata.key == MLX_KEY_DOWN)
			data->camera.pitch += 0.1;
		else if (keydata.key == MLX_KEY_LEFT)
			data->camera.yaw += 0.1;
		else if (keydata.key == MLX_KEY_RIGHT)
			data->camera.yaw -= 0.1;
		update_camera(data);
		render(data);
	}
}
