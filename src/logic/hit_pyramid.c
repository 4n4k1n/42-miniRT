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

static t_vec3	norm_or_zero(t_vec3 v)
{
	double	len;

	len = sqrt(vec3_dot(v, v));
	if (len == 0.0)
		return (v);
	return (vec3_divide(v, len));
}

static void	build_basis(t_vec3 up, t_vec3 *right, t_vec3 *fwd)
{
	t_vec3	t;

	if (fabs(up.x) > 0.9)
		t = (t_vec3){0.0, 1.0, 0.0};
	else
		t = (t_vec3){1.0, 0.0, 0.0};
	*right = norm_or_zero(vec3_cross(up, t));
	*fwd = vec3_cross(*right, up);
}

static int	tri_hit(t_ray *r, t_vec3 v0, t_vec3 v1, t_vec3 v2, double *t, t_vec3 *n)
{
	t_vec3	e1;
	t_vec3	e2;
	t_vec3	p;
	double	det;
	t_vec3	tvec;
	double	u;
	t_vec3	q;
	double	v;
	double	tmp;

	e1 = vec3_sub(v1, v0);
	e2 = vec3_sub(v2, v0);
	*n = norm_or_zero(vec3_cross(e1, e2));
	p = vec3_cross(r->direction, e2);
	det = vec3_dot(e1, p);
	if (fabs(det) < 1e-8)
		return (0);
	tmp = 1.0 / det;
	tvec = vec3_sub(r->origin, v0);
	u = tmp * vec3_dot(tvec, p);
	if (u < 0.0 || u > 1.0)
		return (0);
	q = vec3_cross(tvec, e1);
	v = tmp * vec3_dot(r->direction, q);
	if (v < 0.0 || u + v > 1.0)
		return (0);
	*t = tmp * vec3_dot(e2, q);
	return (*t > 0.0);
}

static void	build_vertices(const t_pyramid *py, t_vec3 *apex, t_vec3 *b0, t_vec3 *b1, t_vec3 *b2, t_vec3 *b3)
{
	t_vec3	up;
	t_vec3	r;
	t_vec3	f;
	double	hh;
	double	hw;
	t_vec3	bc;

	up = norm_or_zero(py->norm);
	build_basis(up, &r, &f);
	hh = py->height * 0.5;
	hw = py->base * 0.5;
	*apex = vec3_add(py->cords, vec3_multiply(up, hh));
	bc = vec3_sub(py->cords, vec3_multiply(up, hh));
	*b0 = vec3_add(vec3_add(bc, vec3_multiply(r, hw)), vec3_multiply(f, hw));
	*b1 = vec3_add(vec3_sub(bc, vec3_multiply(r, hw)), vec3_multiply(f, hw));
	*b2 = vec3_sub(vec3_sub(bc, vec3_multiply(r, hw)), vec3_multiply(f, hw));
	*b3 = vec3_add(vec3_sub(bc, vec3_multiply(r, hw)), vec3_multiply(f, -hw));
}

/**
 * Tests ray/axis-aligned pyramid with given center, axis, base and height
 * Computes UV and tangent basis at the hit for bump mapping
 * Returns 1 if hit, 0 if miss
 */
int	hit_pyramid_obj(const t_pyramid *py, t_ray *r, double tmin, double tmax, t_hit_record *rec)
{
	t_vec3	ap;
	t_vec3	b0;
	t_vec3	b1;
	t_vec3	b2;
	t_vec3	b3;
	double	best_t;
	int		hit;
	double	t;
	t_vec3	n;
	t_vec3	p;
	t_vec3	tmp;
	t_vec3	tan;

	if (!py || !r || !rec)
		return (0);
	build_vertices(py, &ap, &b0, &b1, &b2, &b3);
	best_t = 1.0 / 0.0;
	hit = 0;
	if (tri_hit(r, ap, b0, b1, &t, &n) && t >= tmin && t <= tmax && t < best_t)
		(best_t = t, hit = 1, rec->normal = n);
	if (tri_hit(r, ap, b1, b2, &t, &n) && t >= tmin && t <= tmax && t < best_t)
		(best_t = t, hit = 1, rec->normal = n);
	if (tri_hit(r, ap, b2, b3, &t, &n) && t >= tmin && t <= tmax && t < best_t)
		(best_t = t, hit = 1, rec->normal = n);
	if (tri_hit(r, ap, b3, b0, &t, &n) && t >= tmin && t <= tmax && t < best_t)
		(best_t = t, hit = 1, rec->normal = n);
	if (tri_hit(r, b0, b1, b2, &t, &n) && t >= tmin && t <= tmax && t < best_t)
		(best_t = t, hit = 1, rec->normal = n);
	if (tri_hit(r, b0, b2, b3, &t, &n) && t >= tmin && t <= tmax && t < best_t)
		(best_t = t, hit = 1, rec->normal = n);
	if (!hit)
		return (0);
	rec->t = best_t;
	p = vec3_multiply(r->direction, rec->t);
	rec->p = vec3_add(r->origin, p);
	tmp = (fabs(rec->normal.y) < 0.999) ? (t_vec3){0.0, 1.0, 0.0} : (t_vec3){1.0, 0.0, 0.0};
	tan = vec3_normalize(vec3_cross(tmp, rec->normal));
	rec->tangent = tan;
	rec->bitangent = vec3_cross(rec->normal, tan);
	{
		t_vec3 d = vec3_sub(rec->p, py->cords);
		double u = d.x * rec->tangent.x + d.y * rec->tangent.y + d.z * rec->tangent.z;
		double v = d.x * rec->bitangent.x + d.y * rec->bitangent.y + d.z * rec->bitangent.z;
		rec->u = u - floor(u);
		rec->v = v - floor(v);
	}
	set_face_normal(rec, r, &rec->normal);
	rec->rgb = py->rgb;
	rec->mat = py->mat;
	rec->bump = py->bump;
	return (1);
}
