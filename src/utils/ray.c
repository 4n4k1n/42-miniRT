/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 16:54:31 by apregitz          #+#    #+#             */
/*   Updated: 2025/09/29 21:34:54 by anakin           ###   ########.fr       */
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
	t_rgb	color;

	(void)ray;
	color.r = 0;
	color.g = 0;
	color.b = 0;
	return (color);
}

uint32_t	rgb_to_uint32(t_rgb *color)
{
	return ((uint32_t)((int)color->r << 24 | (int)color->g << 16 | (int)color->b << 8 | 0xf0));
}
