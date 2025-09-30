/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_sphere.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apregitz <apregitz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 11:32:34 by apregitz          #+#    #+#             */
/*   Updated: 2025/09/30 11:55:51 by apregitz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

bool	hit_sphere(t_vec3 *center, double radius, t_ray *ray)
{
	t_vec3	oc;
	double	a;
	double	b;
	double	c;
	double	dis;
	
	(void)center;
	oc = vec3_sub_inline(center, &ray->origin);
	a = vec3_dot_inline(&ray->direction, &ray->direction);
	b = -2.0 * vec3_dot_inline(&ray->direction, &oc);
	c = vec3_dot_inline(&oc, &oc) - radius * radius;
	dis = b * b - 4 * a * c;
	return (dis >= 0);
}
