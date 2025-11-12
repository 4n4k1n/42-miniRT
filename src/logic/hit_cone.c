/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_cone.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 11:05:05 by nweber            #+#    #+#             */
/*   Updated: 2025/11/12 13:30:57 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Tests intersection of a ray with the cone lateral surface.
 * Solves the cone quadratic, orders roots and validates the chosen root
 * against the cone height and provided t range. On success fills rec
 * (t, p, normal, rgb, mat).
 * @param co    cone geometry
 * @param r     ray to test
 * @param range hit t range (tmin/tmax)
 * @param rec   out hit record to fill on success
 * @return 1 if side was hit, 0 otherwise
 */
static int	cone_side_hit(const t_cone *co, t_ray *r, t_hit_range range,
	t_hit_record *rec)
{
	t_cone_calc	cc;
	t_cone_quad	cq;
	double		t0;
	double		t1;
	double		t;

	init_cone_calc(co, r, &cc);
	if (!solve_cone_quad(r, &cc, &cq))
		return (0);
	t0 = (-cq.quad_b - sqrt(cq.dis)) / (2.0 * cq.quad_a);
	t1 = (-cq.quad_b + sqrt(cq.dis)) / (2.0 * cq.quad_a);
	if (t0 > t1)
	{
		t = t0;
		t0 = t1;
		t1 = t;
	}
	t = t0;
	if (t <= range.tmin || t >= range.tmax || !validate_cone_hit(co, r, &cc, t))
		t = t1;
	if (t <= range.tmin || t >= range.tmax || !validate_cone_hit(co, r, &cc, t))
		return (0);
	set_cone_normal(r, &cc, t, rec);
	rec->rgb = co->rgb;
	return (rec->mat = co->mat, 1);
}

/**
 * Tests cone side and base and selects the closest valid hit.
 * Performs side test first, clamps the search range, then tests base.
 * On hit writes the closest record into best.
 * @param co    cone geometry
 * @param r     ray to test
 * @param range hit t range
 * @param best  out best hit record
 * @return 1 if any hit found, 0 otherwise
 */
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
 * Entry point for cone intersection used by the hittable dispatcher.
 * Finds the closest intersection (side or base). On success computes
 * tangent basis, derives UV coordinates and sets bump pointer.
 * @param co    cone geometry
 * @param r     ray to test
 * @param range hit t range
 * @param rec   out hit record to fill on success
 * @return 1 if hit found, 0 otherwise
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
