/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_sphere.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 11:32:34 by apregitz          #+#    #+#             */
/*   Updated: 2025/09/30 17:10:42 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

int	hit_sphere_obj(const t_sphere *s, t_ray *r, double tmin, double tmax, t_hit_record *rec)
{
	const double	radius = s->diameter * 0.5;
	t_vec3			oc;
	double			a;
	double			h;
	double			c;
	double			discriminant;
	double			root;
	t_vec3			tmp;
	t_vec3			outward_normal;

	oc = vec3_sub_inline(&s->cords, &r->origin);
	a = vec3_dot_inline(&r->direction, &r->direction);
	h = vec3_dot_inline(&r->direction, &oc);
	c = vec3_dot_inline(&oc, &oc) - radius * radius;

	discriminant = h * h - a * c;
	if (discriminant < 0.0)
		return (0);

	// Nearest acceptable root
	root = (h - sqrt(discriminant)) / a;
	if (root <= tmin || root >= tmax)
	{
		root = (h + sqrt(discriminant)) / a;
		if (root <= tmin || root >= tmax)
			return (0);
	}

	rec->t = root;
	tmp = vec3_multiply_inline(&r->direction, rec->t);
	rec->p = vec3_add_inline(&r->origin, &tmp);
	outward_normal = vec3_sub_inline(&rec->p, &s->cords);
	outward_normal = vec3_divide_inline(&outward_normal, radius); // unit for sphere
	set_face_normal(rec, r, &outward_normal);

	return (1);
}
