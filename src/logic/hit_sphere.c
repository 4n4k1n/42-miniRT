/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_sphere.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 11:32:34 by apregitz          #+#    #+#             */
/*   Updated: 2025/10/26 15:04:14 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Tests if a ray intersects with a sphere object
 * Uses quadratic formula to solve ray-sphere intersection
 * Formula: discriminant = h² - ac where a=|dir|², h=dir·oc, c=|oc|²-r²
 * Returns 1 if hit, 0 if miss
 */
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

	oc = vec3_sub(s->cords, r->origin);
	a = vec3_dot(r->direction, r->direction);
	h = vec3_dot(r->direction, oc);
	c = vec3_dot(oc, oc) - radius * radius;

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
	tmp = vec3_multiply(r->direction, rec->t);
	rec->p = vec3_add(r->origin, tmp);
	outward_normal = vec3_sub(rec->p, s->cords);
	outward_normal = vec3_divide(outward_normal, radius); // unit for sphere
	set_face_normal(rec, r, &outward_normal);
	rec->rgb = s->rgb;
	rec->mat = s->mat;
	return (1);
}
