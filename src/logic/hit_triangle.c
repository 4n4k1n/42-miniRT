/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_triangle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 21:00:00 by anakin            #+#    #+#             */
/*   Updated: 2025/11/12 12:55:27 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Möller-Trumbore ray-triangle intersection entry.
 * Builds edge vectors and preliminary values, returns 1 on hit.
 * @param tri triangle geometry
 * @param r ray to test
 * @param range valid t range
 * @param rec out hit record
 * @return 1 if intersection found, 0 otherwise
 */
int	hit_triangle_obj(const t_triangle *tri, t_ray *r, t_hit_range range,
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
	return (hit_triangle_test(tri, r, rec,
			(t_tri_hit_ctx){range, (t_tri_calc){e1, e2, p, tvec, det, 0, 0}}));
}

/**
 * Performs the full Möller-Trumbore tests using precomputed ctx.
 * Computes barycentric u,v and t, fills rec on success.
 * @param tri triangle geometry
 * @param r ray
 * @param rec out hit record
 * @param ctx precomputed test context (edges, p, tvec, det, range)
 * @return 1 if hit within range and barycentrics, 0 otherwise
 */
int	hit_triangle_test(const t_triangle *tri, t_ray *r, t_hit_record *rec,
	t_tri_hit_ctx ctx)
{
	double	inv_det;
	double	u;
	t_vec3	q;
	double	v;
	double	t;

	inv_det = 1.0 / ctx.calc.det;
	u = inv_det * vec3_dot(ctx.calc.tvec, ctx.calc.p);
	if (u < 0.0 || u > 1.0)
		return (0);
	q = vec3_cross(ctx.calc.tvec, ctx.calc.e1);
	v = inv_det * vec3_dot(r->direction, q);
	if (v < 0.0 || u + v > 1.0)
		return (0);
	t = inv_det * vec3_dot(ctx.calc.e2, q);
	if (t < ctx.range.tmin || t > ctx.range.tmax)
		return (0);
	rec->t = t;
	rec->p = vec3_add(r->origin, vec3_multiply(r->direction, t));
	rec->normal = vec3_normalize(vec3_cross(ctx.calc.e1, ctx.calc.e2));
	set_face_normal(rec, r, &rec->normal);
	rec->rgb = tri->rgb;
	rec->mat = tri->mat;
	return (set_triangle_tangent_space(rec, &ctx.calc.e1));
}

/**
 * Builds a simple tangent space for the triangle hit.
 * Uses edge e1 as tangent, computes bitangent from normal.
 * Sets u/v to 0 and clears bump pointer.
 * @param rec hit record to update (tangent/bitangent/u/v/bump)
 * @param e1 first edge of the triangle (used for tangent)
 * @return 1 always
 */
int	set_triangle_tangent_space(t_hit_record *rec, t_vec3 *e1)
{
	t_vec3	tan;
	t_vec3	bta;

	tan = vec3_normalize(*e1);
	bta = vec3_cross(rec->normal, tan);
	if (vec3_near_zero(&bta))
		bta = vec3_cross(rec->normal, (t_vec3){1.0, 0.0, 0.0});
	bta = vec3_normalize(bta);
	tan = vec3_cross(bta, rec->normal);
	rec->tangent = tan;
	rec->bitangent = bta;
	rec->u = 0.0;
	rec->v = 0.0;
	rec->bump = NULL;
	return (1);
}
