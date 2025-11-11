/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_cone_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 16:19:22 by nweber            #+#    #+#             */
/*   Updated: 2025/11/11 18:05:58 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

int	cone_base_hit(const t_cone *co, t_ray *r, t_hit_range range,
	t_hit_record *rec)
{
	t_vec3	a;
	t_vec3	center;
	t_vec3	p;
	double	den;
	double	t;

	a = vec3_normalize(co->norm);
	center = vec3_add(co->cords, vec3_multiply(a, co->height * 0.5));
	den = vec3_dot(r->direction, a);
	if (fabs(den) < 1e-12)
		return (0);
	t = vec3_dot(vec3_sub(center, r->origin), a) / den;
	if (t <= range.tmin || t >= range.tmax)
		return (0);
	p = vec3_add(r->origin, vec3_multiply(r->direction, t));
	if (!check_base_distance(p, center, co->diameter * 0.5))
		return (0);
	rec->t = t;
	rec->p = p;
	set_face_normal(rec, r, &a);
	rec->rgb = co->rgb;
	rec->mat = co->mat;
	return (1);
}

void	compute_cone_uv(const t_cone *co, t_hit_record *rec, t_cone_uv *uv)
{
	t_vec3	a;
	t_vec3	r;
	t_vec3	f;
	t_vec3	c;
	t_vec3	k;

	a = vec3_normalize(co->norm);
	build_basis(a, &r, &f);
	if (fabs(vec3_dot(rec->normal, a)) > 0.999)
	{
		c = vec3_add(co->cords, vec3_multiply(a, co->height * 0.5));
		k = vec3_sub(rec->p, c);
		rec->u = 0.5 + 0.5 * (vec3_dot(k, r) / (co->diameter * 0.5));
		rec->v = 0.5 + 0.5 * (vec3_dot(k, f) / (co->diameter * 0.5));
		return ;
	}
	c = vec3_sub(co->cords, vec3_multiply(a, co->height * 0.5));
	k = vec3_sub(rec->p, c);
	uv->x = vec3_dot(k, a);
	k = vec3_sub(k, vec3_multiply(a, uv->x));
	k = vec3_normalize(k);
	rec->u = (atan2(vec3_dot(k, f), vec3_dot(k, r)) + M_PI) / (2.0 * M_PI);
	rec->v = fmax(0.0, fmin(1.0, uv->x / co->height));
}

void	set_cone_tangent_basis(const t_cone *co, t_hit_record *rec)
{
	t_vec3	a;
	t_vec3	c;
	t_vec3	r;

	a = vec3_normalize(co->norm);
	if (fabs(vec3_dot(rec->normal, a)) > 0.999)
	{
		build_basis(a, &rec->tangent, &rec->bitangent);
		return ;
	}
	c = vec3_sub(co->cords, vec3_multiply(a, co->height * 0.5));
	r = vec3_normalize(vec3_sub(rec->p, c));
	rec->tangent = vec3_normalize(vec3_cross(a, r));
	rec->bitangent = a;
}

int	validate_cone_hit(const t_cone *co, t_ray *r, t_cone_calc *cc,
	double t)
{
	t_vec3	q;
	double	s;

	q = vec3_add(r->origin, vec3_multiply(r->direction, t));
	q = vec3_sub(q, cc->apex);
	s = vec3_dot(q, cc->a);
	if (s < 0.0 || s > co->height)
		return (0);
	return (1);
}

void	set_cone_normal(t_ray *r, t_cone_calc *cc, double t,
	t_hit_record *rec)
{
	t_vec3	q;
	t_vec3	n;
	double	s;

	q = vec3_add(r->origin, vec3_multiply(r->direction, t));
	q = vec3_sub(q, cc->apex);
	s = vec3_dot(q, cc->a);
	n = vec3_sub(vec3_multiply(cc->a, s), vec3_multiply(q, cc->c2));
	n = vec3_normalize(n);
	rec->t = t;
	rec->p = vec3_add(r->origin, vec3_multiply(r->direction, t));
	set_face_normal(rec, r, &n);
}
