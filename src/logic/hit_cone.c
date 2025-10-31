/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_cone.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 11:05:05 by nweber            #+#    #+#             */
/*   Updated: 2025/10/29 15:21:00 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static int	cone_side_hit(const t_cone *co, t_ray *r, double tmin, double tmax, t_hit_record *rec)
{
	t_vec3	a;
	double	hh;
	double	rad;
	double	m;
	double	c2;
	t_vec3	apex;
	t_vec3	v;
	double	da;
	double	va;
	double	d2;
	double	dv;
	double	v2;
	double	A;
	double	B;
	double	C;
	double	dis;
	double	t;
	t_vec3	q;
	double	s;
	t_vec3	n;

	a = vec3_normalize(co->norm);
	hh = co->height * 0.5;
	rad = co->diameter * 0.5;
	m = rad / co->height;
	c2 = 1.0 / (1.0 + m * m);
	apex = vec3_sub(co->cords, vec3_multiply(a, hh));
	v = vec3_sub(r->origin, apex);
	da = vec3_dot(r->direction, a);
	va = vec3_dot(v, a);
	d2 = vec3_dot(r->direction, r->direction);
	dv = vec3_dot(r->direction, v);
	v2 = vec3_dot(v, v);
	A = da * da - c2 * d2;
	B = 2.0 * (va * da - c2 * dv);
	C = va * va - c2 * v2;
	if (fabs(A) < 1e-12)
		return (0);
	dis = B * B - 4.0 * A * C;
	if (dis < 0.0)
		return (0);
	t = (-B - sqrt(dis)) / (2.0 * A);
	if (t <= tmin || t >= tmax)
	{
		t = (-B + sqrt(dis)) / (2.0 * A);
		if (t <= tmin || t >= tmax)
			return (0);
	}
	q = vec3_add(r->origin, vec3_multiply(r->direction, t));
	q = vec3_sub(q, apex);
	s = vec3_dot(q, a);
	if (s < 0.0 || s > co->height)
		return (0);
	n = vec3_sub(vec3_multiply(a, s), vec3_multiply(q, c2));
	n = vec3_normalize(n);
	rec->t = t;
	rec->p = vec3_add(r->origin, vec3_multiply(r->direction, t));
	set_face_normal(rec, r, &n);
	rec->rgb = co->rgb;
	rec->mat = co->mat;
	return (1);
}

static int	cone_base_hit(const t_cone *co, t_ray *r, double tmin, double tmax, t_hit_record *rec)
{
	t_vec3	a;
	double	hh;
	t_vec3	center;
	double	den;
	double	t;
	t_vec3	p;
	t_vec3	d;
	double	r2;
	t_vec3	n;

	a = vec3_normalize(co->norm);
	hh = co->height * 0.5;
	center = vec3_add(co->cords, vec3_multiply(a, hh));
	den = vec3_dot(r->direction, a);
	if (fabs(den) < 1e-12)
		return (0);
	t = vec3_dot(vec3_sub(center, r->origin), a) / den;
	if (t <= tmin || t >= tmax)
		return (0);
	p = vec3_add(r->origin, vec3_multiply(r->direction, t));
	d = vec3_sub(p, center);
	r2 = (co->diameter * 0.5) * (co->diameter * 0.5);
	if (vec3_dot(d, d) > r2)
		return (0);
	n = a;
	rec->t = t;
	rec->p = p;
	set_face_normal(rec, r, &n);
	rec->rgb = co->rgb;
	rec->mat = co->mat;
	return (1);
}

/**
 * Computes closest hit on cone (side or base) and sets UV/tangent basis
 * Returns 1 on hit, 0 otherwise
 */
int	hit_cone_obj(const t_cone *co, t_ray *r, double tmin, double tmax, t_hit_record *rec)
{
	t_hit_record	best;
	t_hit_record	tmp;
	int				hit;
	double			best_t;
	t_vec3			best_p;
	t_vec3			best_n;
	t_vec3			a;
	t_vec3			tmpv;
	t_vec3			tan;
	double			u;
	double			v;
	t_vec3			apex;

	hit = 0;
	best_t = tmax;
	if (cone_side_hit(co, r, tmin, best_t, &tmp))
	{
		hit = 1;
		best_t = tmp.t;
		best = tmp;
	}
	if (cone_base_hit(co, r, tmin, best_t, &tmp))
	{
		hit = 1;
		best_t = tmp.t;
		best = tmp;
	}
	if (!hit)
		return (0);
	rec->t = best.t;
	rec->p = best.p;
	a = vec3_normalize(co->norm);
	apex = vec3_add(co->cords, vec3_multiply(a, co->height * 0.5));
	best_p = rec->p;
	best_n = best.normal;
	tmpv = (fabs(a.y) < 0.999) ? (t_vec3){0.0, 1.0, 0.0} : (t_vec3){1.0, 0.0, 0.0};
	tan = vec3_normalize(vec3_cross(tmpv, a));
	rec->tangent = vec3_normalize(vec3_cross(a, vec3_normalize(vec3_sub(best_p, apex))));
	rec->bitangent = a;
	{
		t_vec3 k = vec3_sub(best_p, apex);
		t_vec3 ra = vec3_normalize(vec3_sub(k, vec3_multiply(a, vec3_dot(k, a))));
		double x = vec3_dot(ra, tan);
		double y = vec3_dot(ra, vec3_cross(a, tan));
		u = (atan2(y, x) + M_PI) / (2.0 * M_PI);
		v = fmax(0.0, fmin(1.0, vec3_dot(k, a) / co->height));
	}
	rec->u = u;
	rec->v = v;
	rec->bump = co->bump;
	set_face_normal(rec, r, &best_n);
	rec->rgb = co->rgb;
	rec->mat = co->mat;
	return (1);
}
