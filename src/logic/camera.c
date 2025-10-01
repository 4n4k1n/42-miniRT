/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:01:30 by apregitz          #+#    #+#             */
/*   Updated: 2025/10/01 22:11:22 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static inline t_rgb	rgb_multiply_inline(t_rgb color, double t)
{
	color.r *= t;
	color.g *= t;
	color.b *= t;
	return (color);
}

static inline t_rgb	rgb_modulate_inline(t_rgb a, t_rgb b)
{
	t_rgb out;
	out.r = (a.r * b.r) / 255.999;
	out.g = (a.g * b.g) / 255.999;
	out.b = (a.b * b.b) / 255.999;
	return out;
}

/**
 * Determines the color for a ray by checking sphere intersection
 * If ray hits sphere: calculates surface normal and converts to color
 * If no hit: creates gradient background from white to blue
 * Formula for normal: N = (hit_point - sphere_center) / radius
 */
t_rgb	ray_color(t_ray *ray, t_obj_list *world, int depth)
{
	t_hit_record	rec;
	t_rgb			return_color;
	t_vec3			color_a = (t_vec3){1.0, 1.0, 1.0};
	t_vec3			color_b = (t_vec3){0.5, 0.7, 1.0};
	t_vec3			unit_direction;
	double			len;
	double			a;
	t_vec3			temp1;
	t_vec3			temp2;
	t_vec3			result;

	if (depth <= 0)
		return (t_rgb){0.0, 0.0, 0.0};

	if (world && world_hit(world, ray, 0.001, INFINITY, &rec))
	{
		if (rec.mat)
		{
			t_ray scattered;
			t_rgb attenuation;
			if (rec.mat->scatter(rec.mat, ray, &rec, &attenuation, &scattered))
			{
				t_rgb bounced = ray_color(&scattered, world, depth - 1);
				return rgb_modulate_inline(attenuation, bounced);
			}
			return (t_rgb){0.0, 0.0, 0.0};
		}
		// Fallback: old diffuse bounce if no material set
		t_vec3 direction = random_on_hemisphere(&rec.normal);
		return (rgb_multiply_inline(ray_color(&(t_ray){rec.p, direction}, world, depth - 1), COLOR_INTENSITY));
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
	return_color.r = fmin(fmax(result.x, 0.0), 1.0) * 255.999;
	return_color.g = fmin(fmax(result.y, 0.0), 1.0) * 255.999;
	return_color.b = fmin(fmax(result.z, 0.0), 1.0) * 255.999;
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
	data->width = WIDTH;
	data->height = get_image_height(WIDTH, ASPECT_RATIO);
	data->camera.foc = 1.0;
	data->camera.viewport_height = 2.0;
	data->camera.viewport_width = data->camera.viewport_height * ASPECT_RATIO;
	data->camera.cords = vec3_init_inline(0.0, 0.0, 0.0);
	data->camera.pitch = 0.0;
	data->camera.yaw = 0.0;
	update_camera(data);
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

	i = 0;
	while (i < HEIGHT)
	{
		j = 0;
		while (j < WIDTH)
		{
			if (ANTI_ALIASING)
				mlx_put_pixel(data->img, j, i, monte_carlo_aa(data, &data->aa, i, j));
			else
				mlx_put_pixel(data->img, j, i, without_aa(data, i, j));
			j++;
		}
		printf("%d\n", i);
		i++;
	}
}
