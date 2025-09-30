/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 16:54:31 by apregitz          #+#    #+#             */
/*   Updated: 2025/09/30 13:01:49 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static inline t_vec3	ray_at(t_ray *ray, double t)
{
	t_vec3	temp;

	temp = vec3_multiply_inline(&ray->direction, t);
	return (vec3_add_inline(&ray->origin, &temp));
}

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
	t = hit_sphere(center, 0.5, ray);
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

uint32_t	rgb_to_uint32(t_rgb *color)
{
	return ((uint32_t)((int)color->r << 24 | (int)color->g << 16 | (int)color->b << 8 | 0xf0));
}
