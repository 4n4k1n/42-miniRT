/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 16:54:31 by apregitz          #+#    #+#             */
/*   Updated: 2025/09/30 17:18:00 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

uint32_t	rgb_to_uint32(t_rgb *color)
{
	return ((uint32_t)((int)color->r << 24 | (int)color->g << 16 | (int)color->b << 8 | 0xf0));
}

t_rgb	ray_color(t_ray *ray, t_obj_list *world)
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
	double			r;
	double			g;
	double			b;

	if (world && world_hit(world, ray, 0.001, INFINITY, &rec))
	{
		r = 0.5 * (rec.normal.x + 1.0);
		g = 0.5 * (rec.normal.y + 1.0);
		b = 0.5 * (rec.normal.z + 1.0);
		return_color.r = fmin(fmax(r, 0.0), 1.0) * 255.999;
		return_color.g = fmin(fmax(g, 0.0), 1.0) * 255.999;
		return_color.b = fmin(fmax(b, 0.0), 1.0) * 255.999;
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
	return_color.r = fmin(fmax(result.x, 0.0), 1.0) * 255.999;
	return_color.g = fmin(fmax(result.y, 0.0), 1.0) * 255.999;
	return_color.b = fmin(fmax(result.z, 0.0), 1.0) * 255.999;
	return (return_color);
}
