/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apregitz <apregitz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:01:30 by apregitz          #+#    #+#             */
/*   Updated: 2025/09/30 17:41:03 by apregitz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"



static int	get_image_height(int width, double ra)
{
	int	height;

	height = width / ra;
	if (height < 1)
		return (1);
	return (height);
}

void	init_camera(t_data *data)
{
	t_init_tmp tmp;

	data->width = WIDTH;
	data->height = get_image_height(WIDTH, ASPECT_RATIO);
	data->camera.foc = 1.0;
	data->camera.viewport_height = 2.0;
	data->camera.viewport_width = data->camera.viewport_height * (WIDTH / HEIGHT);
	data->camera.cords = vec3_init_inline(0.0, 0.0, 0.0);
	data->camera.viewport_u = vec3_init_inline(data->camera.viewport_width, 0.0, 0.0);
	data->camera.viewport_v = vec3_init_inline(0.0, data->camera.viewport_height * -1, 0.0);
	data->camera.pixel_delta_u = vec3_divide_inline(&data->camera.viewport_u, WIDTH);
	data->camera.pixel_delta_v = vec3_divide_inline(&data->camera.viewport_v, HEIGHT);
	tmp.t1 = vec3_init_inline(0.0, 0.0, data->camera.foc);
	tmp.t2 = vec3_sub_inline(&data->camera.cords, &tmp.t1);
	tmp.t3 = vec3_divide_inline(&data->camera.viewport_u, 2.0);
	tmp.t4 = vec3_sub_inline(&tmp.t2, &tmp.t3);
	tmp.t5 = vec3_divide_inline(&data->camera.viewport_v, 2.0);
	data->camera.viewport_upper_left = vec3_sub_inline(&tmp.t4, &tmp.t5);
	tmp.t6 = vec3_add_inline(&data->camera.pixel_delta_u, &data->camera.pixel_delta_v);
	tmp.t7 = vec3_multiply_inline(&tmp.t6, 0.5);
	data->camera.pixel00_loc = vec3_add_inline(&data->camera.viewport_upper_left, &tmp.t7);
}

static t_rgb	get_pixel_color(t_camera *cam, int i, int j)
{
	t_pixel_tmp tmp;
	t_rgb       color;

	tmp.u = vec3_multiply_inline(&cam->pixel_delta_u, j);
	tmp.v = vec3_multiply_inline(&cam->pixel_delta_v, i);
	tmp.offset = vec3_add_inline(&tmp.u, &tmp.v);
	tmp.center = vec3_add_inline(&cam->pixel00_loc, &tmp.offset);
	tmp.dir = vec3_sub_inline(&tmp.center, &cam->cords);
	tmp.ray.origin = cam->cords;
	tmp.ray.direction = tmp.dir;
	color = ray_color(&tmp.ray);
	return (color);
}

void	render(t_data *data)
{
	int		i;
	int		j;
	t_rgb	color;

	i = 0;
	while (i < HEIGHT)
	{
		j = 0;
		while (j < WIDTH)
		{
			color = get_pixel_color(&data->camera, i, j);
			color.r = (int)(color.r * 255.0);
			color.g = (int)(color.g * 255.0);
			color.b = (int)(color.b * 255.0);
			mlx_put_pixel(data->img, j, i, rgb_to_uint32(&color));
			j++;
		}
		i++;
	}
}
