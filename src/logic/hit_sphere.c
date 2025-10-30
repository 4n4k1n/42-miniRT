/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_sphere.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 11:32:34 by apregitz          #+#    #+#             */
/*   Updated: 2025/10/30 14:34:15 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static void	calc_sphere_intersection(t_sphere_calc *calc, const t_sphere *s,
		t_ray *r)
{
	calc->radius = s->diameter * 0.5;
	calc->oc = vec3_sub(s->cords, r->origin);
	calc->a = vec3_dot(r->direction, r->direction);
	calc->h = vec3_dot(r->direction, calc->oc);
	calc->c = vec3_dot(calc->oc, calc->oc) - calc->radius * calc->radius;
	calc->discriminant = calc->h * calc->h - calc->a * calc->c;
}

/**
 * Tests if a ray intersects with a sphere object
 * Uses quadratic formula to solve ray-sphere intersection
 * Formula: discriminant = h² - ac where a=|dir|², h=dir·oc, c=|oc|²-r²
 * Returns 1 if hit, 0 if miss
 */
int	hit_sphere_obj(const t_sphere *s, t_ray *r, t_mm mm, t_hit_record *rec)
{
	t_sphere_calc	calc;

	calc_sphere_intersection(&calc, s, r);
	if (calc.discriminant < 0.0)
		return (0);
	calc.sqrtd = sqrt(calc.discriminant);
	calc.root = (calc.h - calc.sqrtd) / calc.a;
	if (calc.root <= mm.tmin || calc.root >= mm.tmax)
	{
		calc.root = (calc.h + calc.sqrtd) / calc.a;
		if (calc.root <= mm.tmin || calc.root >= mm.tmax)
			return (0);
	}
	rec->t = calc.root;
	calc.tmp = vec3_multiply(r->direction, rec->t);
	rec->p = vec3_add(r->origin, calc.tmp);
	calc.outward_normal = vec3_sub(rec->p, s->cords);
	calc.outward_normal = vec3_divide(calc.outward_normal, calc.radius);
	set_face_normal(rec, r, &calc.outward_normal);
	rec->rgb = s->rgb;
	rec->mat = s->mat;
	return (1);
}
