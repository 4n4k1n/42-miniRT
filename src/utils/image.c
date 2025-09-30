/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apregitz <apregitz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 17:53:09 by apregitz          #+#    #+#             */
/*   Updated: 2025/09/30 17:04:09 by apregitz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"
#include "objects.h"

// int	get_image_height(int width, double ra)
// {
// 	int	height;

// 	height = width / ra;
// 	if (height < 1)
// 		return (1);
// 	return (height);
// }

// void	init_camera(t_data *data)
// {
// 	data->width = WIDTH;
// 	data->height = get_image_height(WIDTH, ASPECT_RATIO);
// 	data->camera.foc = 1.0;
// 	data->camera.viewport_height = 2.0;
// 	data->camera.viewport_width = data->camera.viewport_height * (WIDTH / HEIGHT);
// 	data->camera.cords = vec3_init_inline(0.0, 0.0, 0.0);
// 	data->camera.viewport_u = vec3_init_inline(data->camera.viewport_width, 0.0, 0.0);
// 	data->camera.viewport_v = vec3_init_inline(0.0, data->camera.viewport_height * -1, 0.0);
// 	data->camera.pixel_delta_u = vec3_divide_inline(&data->camera.viewport_u, WIDTH);
// 	data->camera.pixel_delta_v = vec3_divide_inline(&data->camera.viewport_v, HEIGHT);
// 	t_vec3 temp1 = vec3_init_inline(0.0, 0.0, data->camera.foc);
// 	t_vec3 temp2 = vec3_sub_inline(&data->camera.cords, &temp1);
// 	t_vec3 temp3 = vec3_divide_inline(&data->camera.viewport_u, 2.0);
// 	t_vec3 temp4 = vec3_sub_inline(&temp2, &temp3);
// 	t_vec3 temp5 = vec3_divide_inline(&data->camera.viewport_v, 2.0);
// 	data->camera.viewport_upper_left = vec3_sub_inline(&temp4, &temp5);

// 	t_vec3 temp6 = vec3_add_inline(&data->camera.pixel_delta_u, &data->camera.pixel_delta_v);
// 	t_vec3 temp7 = vec3_multiply_inline(&temp6, 0.5);
// 	data->camera.pixel00_loc = vec3_add_inline(&data->camera.viewport_upper_left, &temp7);
// }
