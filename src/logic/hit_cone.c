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

static int	cone_side_hit(const t_cone *co, t_ray *r, t_hit_range range,
	t_hit_record *rec)
{
	t_cone_calc	cc;
	t_cone_quad	cq;
	double		t;

	init_cone_calc(co, r, &cc);
	if (!solve_cone_quad(r, &cc, &cq))
		return (0);
	if (!select_cone_root(&cq, range, &t))
		return (0);
	if (!validate_cone_hit(co, r, &cc, t))
		return (0);
	set_cone_normal(r, &cc, t, rec);
	rec->rgb = co->rgb;
	rec->mat = co->mat;
	return (1);
}

static int	test_cone_hits(const t_cone *co, t_ray *r, t_hit_range range,
	t_hit_record *best)
{
	t_hit_record	tmp;
	int				hit;
	double			best_t;

	hit = 0;
	best_t = range.tmax;
	if (cone_side_hit(co, r, range, &tmp))
	{
		hit = 1;
		best_t = tmp.t;
		*best = tmp;
	}
	range.tmax = best_t;
	if (cone_base_hit(co, r, range, &tmp))
	{
		hit = 1;
		*best = tmp;
	}
	return (hit);
}

/**
 * Computes closest hit on cone (side or base) and sets UV/tangent basis
 * Returns 1 on hit, 0 otherwise
 */
int	hit_cone_obj(const t_cone *co, t_ray *r, t_hit_range range,
	t_hit_record *rec)
{
	t_hit_record	best;
	t_cone_uv		uv;

	if (!test_cone_hits(co, r, range, &best))
		return (0);
	*rec = best;
	set_cone_tangent_basis(co, rec);
	uv.tan = vec3_add(co->cords, vec3_multiply(vec3_normalize(co->norm),
				co->height * 0.5));
	compute_cone_uv(co, rec, &uv);
	rec->bump = co->bump;
	return (1);
}
