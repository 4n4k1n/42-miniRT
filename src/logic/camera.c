/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:01:30 by apregitz          #+#    #+#             */
/*   Updated: 2025/09/30 20:47:26 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Calculates a point along a ray at parameter t
 * Formula: point = origin + t * direction
 * Used for ray tracing to find intersection points
 */
static inline t_vec3	ray_at(t_ray *ray, double t)
{
	t_vec3	temp;

	temp = vec3_multiply_inline(&ray->direction, t);
	return (vec3_add_inline(&ray->origin, &temp));
}

/**
 * Determines the color for a ray by checking sphere intersection
 * If ray hits sphere: calculates surface normal and converts to color
 * If no hit: creates gradient background from white to blue
 * Formula for normal: N = (hit_point - sphere_center) / radius
 */
t_rgb	ray_color(t_ray *ray)
{
	t_vec3	color_a = {1.0, 1.0, 1.0};
	t_vec3	color_b = {0.5, 0.7, 1.0};
	t_rgb	return_color;
	t_vec3	unit_direction;
	t_vec3	p;
	t_vec3	center;
	t_vec3	N;
	double	t;
	double	len;
	double	a;
	t_vec3	temp1;
	t_vec3	temp2;
	t_vec3	result;

	center = vec3_init_inline(0.0, 0.0, -1.0);
	t_sphere sphere = {center, 1.0, {0, 0, 0}};
	t_hit_record rec;
	if (hit_sphere_obj(&sphere, ray, 0.001, 1000.0, &rec))
		t = rec.t;
	else
		t = -1.0;
	if (t > 0.0)
	{
		p = ray_at(ray, t);
		N = vec3_sub_inline(&p, &center);
		len = sqrt(vec3_dot_inline(&N, &N));
		if (len != 0.0)
			N = vec3_divide_inline(&N, len);
		return_color.r = 0.5 * (N.x + 1.0);
		return_color.g = 0.5 * (N.y + 1.0);
		return_color.b = 0.5 * (N.z + 1.0);
		return (return_color);
	}
	len = sqrt(vec3_dot_inline(&ray->direction, &ray->direction));
	if (len != 0.0)
		unit_direction = vec3_divide_inline(&ray->direction, len);
	else
		unit_direction = vec3_cpy_inline(&ray->direction);
	a = 0.5 * (unit_direction.y + 1.0);
	temp1 = vec3_multiply_inline(&color_a, 1.0 - a);
	temp2 = vec3_multiply_inline(&color_b, a);
	result = vec3_add_inline(&temp1, &temp2);
	return_color.r = result.x;
	return_color.g = result.y;
	return_color.b = result.z;
	return (return_color);
}

/**
 * Calculates image height based on width and aspect ratio
 * Formula: height = width / aspect_ratio
 * Ensures minimum height of 1 pixel
 */
static int	get_image_height(int width, double ra)
{
	int	height;

	height = width / ra;
	if (height < 1)
		return (1);
	return (height);
}

/**
 * Initializes camera parameters for ray tracing
 * Sets up viewport dimensions, pixel deltas, and starting pixel location
 * Creates coordinate system for converting screen pixels to world rays
 */
void	init_camera(t_data *data)
{
	t_init_tmp tmp;

	data->width = WIDTH;
	data->height = get_image_height(WIDTH, ASPECT_RATIO);
	data->camera.foc = 1.0;
	data->camera.viewport_height = 2.0;
	data->camera.viewport_width = data->camera.viewport_height * ASPECT_RATIO;
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

/**
 * Calculates color for a specific pixel at screen coordinates (i,j)
 * Converts screen coordinates to world ray and traces it
 * Returns RGB color value for that pixel
 */
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

/**
 * Main rendering loop that processes all pixels in the image
 * Iterates through each pixel, calculates its color, and draws it
 * Converts color values from [0,1] range to [0,255] for display
 */
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
