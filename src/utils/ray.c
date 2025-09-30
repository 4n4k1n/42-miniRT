/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 16:54:31 by apregitz          #+#    #+#             */
/*   Updated: 2025/09/29 22:04:39 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

inline t_vec3	ray_at(t_ray *ray, double t)
{
	t_vec3 temp = vec3_multiply_inline(&ray->direction, t);
	return (vec3_add_inline(&ray->origin, &temp));
}

t_rgb	ray_color(t_ray *ray)
{
	t_vec3	color_a = {1.0, 1.0, 1.0};
	t_vec3	color_b = {0.5, 0.7, 1.0};
	t_rgb	return_color;
	t_vec3	unit_direction;
	t_vec3	result;
	t_vec3	temp1;
	t_vec3	temp2;
	double	a;
	
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
