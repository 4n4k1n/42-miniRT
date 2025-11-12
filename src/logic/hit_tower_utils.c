/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_tower_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 23:00:00 by anakin            #+#    #+#             */
/*   Updated: 2025/11/12 12:58:17 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Validates and normalizes the cylinder axis.
 * Computes axis length and normalized axis into ch. Fails if zero-length.
 * @param cyl cylinder geometry
 * @param ch output hit helper (writes axis_len and axis)
 * @return 1 valid, 0 invalid
 */
int	cyl_valid_axis(const t_cylinder *cyl, t_cyl_hit *ch)
{
	ch->axis_len = vec3_sqrt(cyl->norm);
	if (ch->axis_len == 0.0)
		return (0);
	ch->axis = vec3_normalize(cyl->norm);
	return (1);
}

/**
 * Selects a valid quadratic root for the cylindrical side hit.
 * Tries the near root, falls back to the far root and ensures s within half_h.
 * On success fills ch->root and ch->point.
 * @param ch precomputed quadratic values (a,h,c,disc,k_dot_a,d_dot_a ...)
 * @param r ray (used to compute point)
 * @param range valid t range
 * @return 1 a valid root was selected, 0 otherwise
 */
int	cyl_select_root(t_cyl_hit *ch, t_ray *r, t_hit_range range)
{
	double	sq;

	sq = sqrt(ch->disc);
	ch->root = (-ch->h - sq) / ch->a;
	if (ch->root <= range.tmin || ch->root >= range.tmax)
		ch->root = (-ch->h + sq) / ch->a;
	if (ch->root <= range.tmin || ch->root >= range.tmax)
		return (0);
	ch->s = ch->k_dot_a + ch->root * ch->d_dot_a;
	if (fabs(ch->s) > ch->half_h)
	{
		ch->root = (-ch->h + sq) / ch->a;
		if (ch->root <= range.tmin || ch->root >= range.tmax)
			return (0);
		ch->s = ch->k_dot_a + ch->root * ch->d_dot_a;
		if (fabs(ch->s) > ch->half_h)
			return (0);
	}
	ch->point = vec3_add(r->origin, vec3_multiply(r->direction, ch->root));
	return (1);
}

/**
 * Checks a cap intersection param and computes t to the cap plane.
 * Uses sign for top/bottom and returns t>0 when a valid intersection exists.
 * @param ctx cap check context (cyl, top flag)
 * @param r ray to test
 * @param ch cylinder hit helper (uses axis and half_h)
 * @return 1 potential cap intersection (t>0), 0 otherwise
 */
int	cyl_cap_hit_check(t_cyl_cap_ctx *ctx, t_ray *r, t_cyl_hit *ch)
{
	double	sign;
	double	denom;
	t_vec3	tmp;

	if (ctx->top)
		sign = 1.0;
	else
		sign = -1.0;
	ctx->center = vec3_add(ctx->cyl->cords,
			vec3_multiply(ch->axis, ch->half_h * sign));
	denom = vec3_dot(r->direction, ch->axis) * sign;
	if (fabs(denom) < 1e-9)
		return (0);
	tmp = vec3_sub(ctx->center, r->origin);
	ctx->t = vec3_dot(tmp, ch->axis) * sign / denom;
	return (ctx->t > 0.0);
}

/**
 * Tests and records a closer hit when found.
 * Updates best with ch values if ch->root is within range and closer.
 * @param ch candidate hit
 * @param range valid t range
 * @param best best-hit accumulator to update
 */
void	check_best_hit(t_cyl_hit *ch, t_hit_range range, t_cyl_best *best)
{
	if (ch->root >= range.tmin && ch->root <= range.tmax
		&& ch->root < best->t)
	{
		best->hit = 1;
		best->t = ch->root;
		best->p = ch->point;
		best->n = ch->outward;
	}
}

/**
 * Computes UV coordinates and auxiliary vectors for a cylinder hit.
 * Builds a stable tangent from axis and a tmp vector, computes u from atan2
 * and v from height projection.
 * @param cyl cylinder geometry
 * @param ch cylinder hit helper (uses axis and s)
 * @param best_p hit point on the surface
 * @param rec hit record to write u/v
 */
void	compute_cyl_uv(const t_cylinder *cyl, t_cyl_hit *ch, t_vec3 best_p,
	t_hit_record *rec)
{
	t_vec3	base;
	t_vec3	k;
	t_vec3	tan;
	t_vec3	tmp;
	t_vec3	rp;

	tmp = (t_vec3){0.0, 1.0, 0.0};
	if (fabs(ch->axis.y) >= 0.999)
		tmp = (t_vec3){1.0, 0.0, 0.0};
	tan = vec3_normalize(vec3_cross(tmp, ch->axis));
	base = vec3_sub(cyl->cords, vec3_multiply(ch->axis, cyl->height * 0.5));
	k = vec3_sub(best_p, base);
	ch->s = fmax(0.0, fmin(cyl->height, vec3_dot(k, ch->axis)));
	rp = vec3_normalize(vec3_sub(k, vec3_multiply(ch->axis, ch->s)));
	rec->u = (atan2(vec3_dot(rp, vec3_cross(ch->axis, tan)),
				vec3_dot(rp, tan)) + M_PI) / (2.0 * M_PI);
	rec->v = ch->s / cyl->height;
}
