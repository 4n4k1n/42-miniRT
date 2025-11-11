/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_cone_calc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 00:00:00 by nweber            #+#    #+#             */
/*   Updated: 2025/11/11 00:00:00 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

void	init_cone_calc(const t_cone *co, t_ray *r, t_cone_calc *cc)
{
	cc->a = vec3_normalize(co->norm);
	cc->hh = co->height * 0.5;
	cc->rad = co->diameter * 0.5;
	cc->m = cc->rad / co->height;
	cc->c2 = 1.0 / (1.0 + cc->m * cc->m);
	cc->apex = vec3_sub(co->cords, vec3_multiply(cc->a, cc->hh));
	cc->v = vec3_sub(r->origin, cc->apex);
}

int	solve_cone_quad(t_ray *r, t_cone_calc *cc, t_cone_quad *cq)
{
	cq->da = vec3_dot(r->direction, cc->a);
	cq->va = vec3_dot(cc->v, cc->a);
	cq->d2 = vec3_dot(r->direction, r->direction);
	cq->dv = vec3_dot(r->direction, cc->v);
	cq->v2 = vec3_dot(cc->v, cc->v);
	cq->quad_a = cq->da * cq->da - cc->c2 * cq->d2;
	cq->quad_b = 2.0 * (cq->va * cq->da - cc->c2 * cq->dv);
	cq->quad_c = cq->va * cq->va - cc->c2 * cq->v2;
	if (fabs(cq->quad_a) < 1e-12)
		return (0);
	cq->dis = cq->quad_b * cq->quad_b - 4.0 * cq->quad_a * cq->quad_c;
	if (cq->dis < 0.0)
		return (0);
	return (1);
}

int	select_cone_root(t_cone_quad *cq, t_hit_range range, double *t)
{
	*t = (-cq->quad_b - sqrt(cq->dis)) / (2.0 * cq->quad_a);
	if (*t <= range.tmin || *t >= range.tmax)
	{
		*t = (-cq->quad_b + sqrt(cq->dis)) / (2.0 * cq->quad_a);
		if (*t <= range.tmin || *t >= range.tmax)
			return (0);
	}
	return (1);
}

int	check_base_distance(t_vec3 p, t_vec3 center, double radius)
{
	t_vec3	d;
	double	r2;

	d = vec3_sub(p, center);
	r2 = radius * radius;
	if (vec3_dot(d, d) > r2)
		return (0);
	return (1);
}
