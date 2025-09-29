/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apregitz <apregitz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 16:54:31 by apregitz          #+#    #+#             */
/*   Updated: 2025/09/29 17:52:06 by apregitz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

inline t_vec3	ray_at(t_ray *ray, double t)
{
	return (vec3_add_inline(ray->origin, vec3_multiply_inline(ray->direction, t)));
}

inline t_rgb	ray_color(t_ray *ray)
{
	t_rgb	color;

	(void)ray;
	color.r = 0;
	color.g = 0;
	color.b = 0;
	return (color);
}
