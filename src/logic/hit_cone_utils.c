/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_cone_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 16:19:22 by nweber            #+#    #+#             */
/*   Updated: 2025/11/11 16:19:25 by nweber           ###   ########.fr       */
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
	t_vec3	apex;

	a = vec3_normalize(co->norm);
	apex = vec3_sub(co->cords, vec3_multiply(a, co->height * 0.5));
	uv->tan = apex;
	uv->k = vec3_sub(rec->p, uv->tan);
	uv->ra = vec3_normalize(vec3_sub(uv->k,
				vec3_multiply(a, vec3_dot(uv->k, a))));
	uv->x = vec3_dot(uv->ra, uv->tan);
	uv->y = vec3_dot(uv->ra, vec3_cross(a, uv->tan));
	rec->u = (atan2(uv->y, uv->x) + M_PI) / (2.0 * M_PI);
	rec->v = fmax(0.0, fmin(1.0, vec3_dot(uv->k, a) / co->height));
}

void	set_cone_tangent_basis(const t_cone *co, t_hit_record *rec)
{
	t_vec3	a;
	t_vec3	apex;

	a = vec3_normalize(co->norm);
	apex = vec3_sub(co->cords, vec3_multiply(a, co->height * 0.5));
	rec->tangent = vec3_normalize(vec3_cross(a,
				vec3_normalize(vec3_sub(rec->p, apex))));
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
