/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_triangle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 21:00:00 by anakin            #+#    #+#             */
/*   Updated: 2025/10/30 14:34:25 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Möller-Trumbore ray-triangle intersection algorithm
 * Fast intersection test using barycentric coordinates
 * Returns 1 if hit, 0 otherwise
 */
int	hit_triangle_obj(const t_triangle *tri, t_ray *r, double tmin, double tmax,
		t_hit_record *rec)
{
	t_vec3	e1;
	t_vec3	e2;
	t_vec3	p;
	double	det;
	t_vec3	tvec;

	if (!tri || !r || !rec)
		return (0);
	e1 = vec3_sub(tri->v1, tri->v0);
	e2 = vec3_sub(tri->v2, tri->v0);
	p = vec3_cross(r->direction, e2);
	det = vec3_dot(e1, p);
	if (fabs(det) < 1e-8)
		return (0);
	tvec = vec3_sub(r->origin, tri->v0);
	return (hit_triangle_test(tri, r, tmin, tmax, rec, (t_tri_calc){e1, e2, p,
			tvec, det, 0, 0}));
}

int	hit_triangle_test(const t_triangle *tri, t_ray *r, double tmin, double tmax,
		t_hit_record *rec, t_tri_calc calc)
{
	double	inv_det;
	double	u;
	t_vec3	q;
	double	v;
	double	t;

	inv_det = 1.0 / calc.det;
	u = inv_det * vec3_dot(calc.tvec, calc.p);
	if (u < 0.0 || u > 1.0)
		return (0);
	q = vec3_cross(calc.tvec, calc.e1);
	v = inv_det * vec3_dot(r->direction, q);
	if (v < 0.0 || u + v > 1.0)
		return (0);
	t = inv_det * vec3_dot(calc.e2, q);
	if (t < tmin || t > tmax)
		return (0);
	rec->t = t;
	rec->p = vec3_add(r->origin, vec3_multiply(r->direction, t));
	rec->normal = vec3_normalize(vec3_cross(calc.e1, calc.e2));
	set_face_normal(rec, r, &rec->normal);
	rec->rgb = tri->rgb;
	rec->mat = tri->mat;
	return (1);
}
