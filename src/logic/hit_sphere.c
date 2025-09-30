/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_sphere.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 11:32:34 by apregitz          #+#    #+#             */
/*   Updated: 2025/09/30 12:37:52 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

int	hit_sphere(t_vec3 center, double radius, t_ray *r)
{
	t_vec3	oc;
	double	a;
	double	b;
	double	c;
	double	discriminant;

	oc = vec3_sub_inline(&center, &r->origin);
	a = vec3_dot_inline(&r->direction, &r->direction);
	b = -2.0 * vec3_dot_inline(&r->direction, &oc);
	c = vec3_dot_inline(&oc, &oc) - radius * radius;
	discriminant = b * b - 4.0 * a * c;
	return (discriminant >= 0.0);
}
