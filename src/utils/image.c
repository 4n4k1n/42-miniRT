/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 17:53:09 by apregitz          #+#    #+#             */
/*   Updated: 2025/09/30 19:16:45 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"
#include "objects.h"

int	get_image_height(int width, double ra)
{
	int	height;

	height = width / ra;
	if (height < 1)
		return (1);
	return (height);
}

void	init_camera(t_data *data)
{
	data->width = WIDTH;
	data->height = HEIGHT;
	data->camera.cords = vec3_init_inline(0.0, 0.0, 0.0);
	data->camera.foc = 1.0;
	data->camera.viewport_height = 2.0;
	data->camera.viewport_width = data->camera.viewport_height * ((double)data->width / (double)data->height);
	data->camera.viewport_u = vec3_init_inline(data->camera.viewport_width, 0.0, 0.0);
	data->camera.viewport_v = vec3_init_inline(0.0, -data->camera.viewport_height, 0.0);
	data->camera.pixel_delta_u = vec3_multiply_inline(&data->camera.viewport_u, 1.0 / (double)data->width);
	data->camera.pixel_delta_v = vec3_multiply_inline(&data->camera.viewport_v, 1.0 / (double)data->height);
	t_vec3 focal = vec3_init_inline(0.0, 0.0, data->camera.foc);
	t_vec3 c_minus_f = vec3_sub_inline(&data->camera.cords, &focal);
	t_vec3 half_u = vec3_multiply_inline(&data->camera.viewport_u, 0.5);
	t_vec3 half_v = vec3_multiply_inline(&data->camera.viewport_v, 0.5);
	t_vec3 ul = vec3_sub_inline(&c_minus_f, &half_u);
	data->camera.viewport_upper_left = vec3_sub_inline(&ul, &half_v);
	t_vec3 sum = vec3_add_inline(&data->camera.pixel_delta_u, &data->camera.pixel_delta_v);
	t_vec3 half = vec3_multiply_inline(&sum, 0.5);
	data->camera.pixel00_loc = vec3_add_inline(&data->camera.viewport_upper_left, &half);
	if (data->camera.samples_per_pixel <= 0)
		data->camera.samples_per_pixel = 1;
	data->camera.pixel_samples_scale = 1.0 / (double)data->camera.samples_per_pixel;
}
