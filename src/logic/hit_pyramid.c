/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_pyramid.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 11:31:15 by nweber            #+#    #+#             */
/*   Updated: 2025/10/29 15:20:40 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static int	tri_hit(t_ray *r, t_vec3 tri[3], double *t, t_vec3 *n)
{
	t_py_tri_calc	calc;
	t_py_tri_uv		uv;

	calc.e1 = vec3_sub(tri[1], tri[0]);
	calc.e2 = vec3_sub(tri[2], tri[0]);
	*n = norm_or_zero(vec3_cross(calc.e1, calc.e2));
	calc.p = vec3_cross(r->direction, calc.e2);
	calc.det = vec3_dot(calc.e1, calc.p);
	if (fabs(calc.det) < 1e-8)
		return (0);
	calc.inv_det = 1.0 / calc.det;
	uv.tvec = vec3_sub(r->origin, tri[0]);
	uv.u = calc.inv_det * vec3_dot(uv.tvec, calc.p);
	uv.q = vec3_cross(uv.tvec, calc.e1);
	uv.v = calc.inv_det * vec3_dot(r->direction, uv.q);
	if (!check_uv_bounds(&uv))
		return (0);
	*t = calc.inv_det * vec3_dot(calc.e2, uv.q);
	return (*t > 0.0);
}

static void	build_vertices(const t_pyramid *py, t_py_verts *v)
{
	t_vec3		up;
	double		hh;
	t_py_basis	basis;

	up = norm_or_zero(py->norm);
	build_basis(up, &basis.r, &basis.f);
	hh = py->height * 0.5;
	basis.hw = py->base * 0.5;
	v->apex = vec3_add(py->cords, vec3_multiply(up, hh));
	basis.bc = vec3_sub(py->cords, vec3_multiply(up, hh));
	compute_base_corners(&basis, v);
}

static void	check_tri_hit(t_ray *r, t_vec3 tri[3], t_hit_range range,
	t_py_best *best)
{
	double	t;
	t_vec3	n;

	if (tri_hit(r, tri, &t, &n) && t >= range.tmin && t <= range.tmax
		&& t < best->t)
	{
		best->t = t;
		best->hit = 1;
		best->n = n;
	}
}

static void	test_all_faces(t_ray *r, t_py_verts *v, t_hit_range range,
	t_py_best *best)
{
	t_vec3	tri[3];

	tri[0] = v->apex;
	tri[1] = v->b0;
	tri[2] = v->b1;
	check_tri_hit(r, tri, range, best);
	tri[1] = v->b1;
	tri[2] = v->b2;
	check_tri_hit(r, tri, range, best);
	tri[1] = v->b2;
	tri[2] = v->b3;
	check_tri_hit(r, tri, range, best);
	tri[1] = v->b3;
	tri[2] = v->b0;
	check_tri_hit(r, tri, range, best);
	tri[0] = v->b0;
	tri[1] = v->b1;
	tri[2] = v->b2;
	check_tri_hit(r, tri, range, best);
	tri[1] = v->b2;
	tri[2] = v->b3;
	check_tri_hit(r, tri, range, best);
}

/**
 * Tests ray/axis-aligned pyramid with given center, axis, base and height
 * Computes UV and tangent basis at the hit for bump mapping
 * Returns 1 if hit, 0 if miss
 */
int	hit_pyramid_obj(const t_pyramid *py, t_ray *r, t_hit_range range,
	t_hit_record *rec)
{
	t_py_verts	v;
	t_py_best	best;

	if (!py || !r || !rec)
		return (0);
	build_vertices(py, &v);
	best.t = 1.0 / 0.0;
	best.hit = 0;
	test_all_faces(r, &v, range, &best);
	if (!best.hit)
		return (0);
	rec->normal = best.n;
	rec->t = best.t;
	rec->p = vec3_add(r->origin, vec3_multiply(r->direction, rec->t));
	set_tangent_basis(rec);
	compute_uv(py, rec);
	set_face_normal(rec, r, &rec->normal);
	rec->rgb = py->rgb;
	rec->mat = py->mat;
	rec->bump = py->bump;
	return (1);
}
