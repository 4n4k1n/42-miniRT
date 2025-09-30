/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_sphere.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 11:32:34 by apregitz          #+#    #+#             */
/*   Updated: 2025/09/30 12:59:25 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

double	hit_sphere(t_vec3 center, double radius, t_ray *r)
{
	t_vec3	oc;
	double	a;
	double	h;
	double	c;
	double	discriminant;

	oc = vec3_sub_inline(&center, &r->origin);
	a = vec3_dot_inline(&r->direction, &r->direction);
	h = vec3_dot_inline(&r->direction, &oc);
	c = vec3_dot_inline(&oc, &oc) - radius * radius;
	discriminant = h * h - a * c;
	if (discriminant < 0.0)
		return (-1.0);
	return ((h - sqrt(discriminant)) / a);
}
