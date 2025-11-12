/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_cone_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 16:19:22 by nweber            #+#    #+#             */
/*   Updated: 2025/11/12 13:32:24 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Tests intersection with the cone base (circular cap).
 * Computes plane intersection against the base center and verifies the
 * hit lies within the base radius.
 * @param co    cone geometry (cords, norm, height, diameter)
 * @param r     ray to test
 * @param range valid t range (tmin/tmax)
 * @param rec   out hit record to fill on success (t, p, normal, rgb, mat)
 * @return 1 if the base was hit, 0 otherwise
 */
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

/**
 * Computes UV coordinates for a hit on the cone (base or lateral surface).
 * Builds a local orthonormal basis from the cone axis and projects the
 * hit displacement to derive u (angle) and v (height fraction).
 * Special-case: when the surface normal is nearly parallel to the axis,
 * the function computes planar coordinates around the base center.
 * @param co  cone geometry (cords, norm, height, diameter)
 * @param rec hit record with p and normal; writes u and v
 * @param uv  temporary UV context containing auxiliary values
 */
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

/**
 * Builds a tangent/bitangent basis at the cone hit point.
 * If the normal is nearly parallel to the cone axis, use build_basis;
 * otherwise compute tangent from radial direction and set bitangent = axis.
 * @param co  cone geometry (uses norm)
 * @param rec hit record (reads p, normal; writes tangent and bitangent)
 */
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

/**
 * Validates that the parametric position along the cone axis for a given
 * candidate t lies within the cone height [0, height].
 * @param co cone geometry (height)
 * @param r ray used to compute point at t
 * @param cc precomputed cone calc context (apex, a)
 * @param t candidate t value along the ray
 * @return 1 when the hit is within the cone height, 0 otherwise
 */
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

/**
 * Computes and sets the outward normal for a cone lateral intersection.
 * Uses the cone calc context to derive the analytic normal and writes
 * t, p, and oriented normal into the hit record.
 * @param r  ray that produced the hit
 * @param cc cone calc context (apex, a, c2)
 * @param t  hit distance along r
 * @param rec hit record to fill (t, p, normal, front_face)
 */
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
