/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_tower.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 11:32:34 by apregitz          #+#    #+#             */
/*   Updated: 2025/11/12 12:56:46 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Tests a cylinder cap (top or bottom) for intersection and fills ch.
 * Uses cyl_cap_hit_check to compute t/center; then verifies radius.
 * @param cyl cylinder geometry
 * @param r ray to test
 * @param ch out hit container (filled on success)
 * @param top 1 = top cap, 0 = bottom cap
 * @return 1 hit found, 0 otherwise
 */
static int	cyl_cap_hit(const t_cylinder *cyl, t_ray *r, t_cyl_hit *ch,
	int top)
{
	t_cyl_cap_ctx	ctx;
	t_vec3			cp;
	t_vec3			tmp;

	ctx.cyl = cyl;
	ctx.top = top;
	if (!cyl_cap_hit_check(&ctx, r, ch))
		return (0);
	cp = vec3_add(r->origin, vec3_multiply(r->direction, ctx.t));
	tmp = vec3_sub(cp, ctx.center);
	if (vec3_dot(tmp, tmp) > ch->radius * ch->radius)
		return (0);
	ch->root = ctx.t;
	ch->point = cp;
	ch->outward = ch->axis;
	if (!top)
		ch->outward = vec3_multiply(ch->outward, -1.0);
	return (1);
}

/**
 * Tests the cylindrical side for intersection and computes shading info.
 * Solves quadratic on the perpendicular components and selects a root.
 * Fills ch->point, outward normal and other intermediate values.
 * @param cyl cylinder geometry
 * @param r ray to test
 * @param range valid t range
 * @param ch out hit container (filled on success)
 * @return 1 hit on side, 0 otherwise
 */
static int	cyl_side_hit(const t_cylinder *cyl, t_ray *r, t_hit_range range,
	t_cyl_hit *ch)
{
	t_vec3	d_perp;
	t_vec3	k_perp;
	t_vec3	q;

	ch->radius = cyl->diameter * 0.5;
	ch->half_h = cyl->height * 0.5;
	ch->k = vec3_sub(r->origin, cyl->cords);
	ch->d_dot_a = vec3_dot(r->direction, ch->axis);
	ch->k_dot_a = vec3_dot(ch->k, ch->axis);
	d_perp = vec3_sub(r->direction, vec3_multiply(ch->axis, ch->d_dot_a));
	k_perp = vec3_sub(ch->k, vec3_multiply(ch->axis, ch->k_dot_a));
	ch->a = vec3_dot(d_perp, d_perp);
	ch->h = vec3_dot(d_perp, k_perp);
	ch->c = vec3_dot(k_perp, k_perp) - ch->radius * ch->radius;
	ch->disc = ch->h * ch->h - ch->a * ch->c;
	if (ch->a <= 1e-12 || ch->disc < 0.0)
		return (0);
	if (!cyl_select_root(ch, r, range))
		return (0);
	q = vec3_sub(ch->point, cyl->cords);
	ch->outward = vec3_sub(q, vec3_multiply(ch->axis, vec3_dot(q, ch->axis)));
	ch->outward = vec3_normalize(ch->outward);
	return (1);
}

/**
 * Populate hit record fields for the selected cylinder hit.
 * Computes tangent/bitangent, UV, bump pointer, face orientation, rgb and mat.
 * @param ctx context with cyl, ray, ch (cap/side) and best hit info
 * @param rec output hit record to fill
 */
static void	set_cyl_record(t_cyl_record_ctx *ctx, t_hit_record *rec)
{
	t_vec3	proj;

	rec->t = ctx->best->t;
	rec->p = ctx->best->p;
	proj = vec3_add(ctx->cyl->cords, vec3_multiply(ctx->ch->axis,
				vec3_dot(vec3_sub(ctx->best->p, ctx->cyl->cords),
					ctx->ch->axis)));
	rec->tangent = vec3_normalize(vec3_cross(ctx->ch->axis,
				vec3_normalize(vec3_sub(ctx->best->p, proj))));
	rec->bitangent = ctx->ch->axis;
	compute_cyl_uv(ctx->cyl, ctx->ch, ctx->best->p, rec);
	rec->bump = ctx->cyl->bump;
	set_face_normal(rec, ctx->r, &ctx->best->n);
	rec->rgb = ctx->cyl->rgb;
	rec->mat = ctx->cyl->mat;
}

/**
 * Cylinder (tower) intersection entry point used by hittable.
 * Tests side and both caps, selects the nearest valid hit and fills rec.
 * @param cyl cylinder geometry
 * @param r ray to test
 * @param range valid t range (tmin/tmax)
 * @param rec out hit record (filled on closest hit)
 * @return 1 if hit found, 0 otherwise
 */
int	hit_cylinder_obj(const t_cylinder *cyl, t_ray *r, t_hit_range range,
	t_hit_record *rec)
{
	t_cyl_hit			ch;
	t_cyl_best			best;
	t_cyl_record_ctx	ctx;

	best.hit = 0;
	best.t = range.tmax;
	if (!cyl_valid_axis(cyl, &ch))
		return (0);
	if (cyl_side_hit(cyl, r, range, &ch))
		check_best_hit(&ch, range, &best);
	if (cyl_cap_hit(cyl, r, &ch, 1))
		check_best_hit(&ch, range, &best);
	if (cyl_cap_hit(cyl, r, &ch, 0))
		check_best_hit(&ch, range, &best);
	if (!best.hit)
		return (0);
	ctx = (t_cyl_record_ctx){cyl, r, &ch, &best};
	set_cyl_record(&ctx, rec);
	return (1);
}
