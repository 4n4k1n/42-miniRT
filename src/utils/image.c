/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apregitz <apregitz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 17:53:09 by apregitz          #+#    #+#             */
/*   Updated: 2025/09/29 19:46:41 by apregitz         ###   ########.fr       */
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
	data->height = get_image_height(WIDTH, ASPECT_RATIO);
	data->camera.foc = 1.0;
	data->camera.viewport_height = 2.0;
	data->camera.viewport_width = data->camera.viewport_height * (WIDTH / HEIGHT);
	data->camera.center = vec3_init(0.0, 0.0, 0.0);
	data->camera.viewport_u = vec3_init(data->camera.viewport_width, 0.0, 0.0);
	data->camera.viewport_v = vec3_init(0.0, data->camera.viewport_height * -1, 0.0);
	data->camera.pixel_delta_u = vec3_divide_inline(data->camera.viewport_u, WIDTH);
	data->camera.pixel_delta_v = vec3_divide_inline(data->camera.viewport_v, height);
	data->camera.viewport_upper_left = vec3_sub_inline(vec3_sub_inline(data->camera.center, vec3_init(0.0, 0.0, data->camera.foc), vec3_divide_inline(data->camera.viewport_u, 2.0)), vec3_divide(data->camera.viewport_v, 2.0));
	data->camera.pixel00_loc = vec3_add_inline(data->camera.viewport_upper_left, vec3_multiply_inline(vec3_add_inline(data->camera.pixel_delta_u, data->camera.pixel_delta_v), 0.5));
}
