/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_plane.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 11:32:34 by apregitz          #+#    #+#             */
/*   Updated: 2025/11/12 13:13:17 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Computes planar UV coordinates for a hit point on the plane.
 * Projects the hit displacement from the plane origin onto the provided
 * tangent and the record's bitangent, then wraps each coordinate into [0,1).
 * @param hp   hit position on the plane
 * @param pl   plane geometry (uses cords as plane origin)
 * @param tan  tangent vector for the plane surface
 * @param rec  hit record to write u/v
 */
static void	compute_plane_uv(t_vec3 hp, const t_plane *pl, t_vec3 tan,
	t_hit_record *rec)
{
	t_vec3	d;
	double	u;
	double	v;

	d = vec3_sub(hp, pl->cords);
	u = d.x * tan.x + d.y * tan.y + d.z * tan.z;
	v = d.x * rec->bitangent.x + d.y * rec->bitangent.y
		+ d.z * rec->bitangent.z;
	rec->u = u - floor(u);
	rec->v = v - floor(v);
}

/**
 * Builds a stable tangent / bitangent basis for the plane surface.
 * Chooses a fallback axis to avoid degeneracy when the outward normal is
 * nearly aligned with the world up vector.
 * @param outward outward unit normal at the plane
 * @param rec     hit record to write tangent and bitangent
 */
static void	compute_plane_tangent_basis(t_vec3 outward, t_hit_record *rec)
{
	t_vec3	tmp;
	t_vec3	tan;

	if (fabs(outward.y) < 0.999)
		tmp = (t_vec3){0.0, 1.0, 0.0};
	else
		tmp = (t_vec3){1.0, 0.0, 0.0};
	tan = vec3_normalize(vec3_cross(tmp, outward));
	rec->tangent = tan;
	rec->bitangent = vec3_cross(outward, tan);
}

/**
 * Tests if a ray intersects with a plane.
 * Computes hit distance, hit point, tangent basis and UV for bump/texturing.
 * Fills rec with bump, rgb, mat and sets face orientation.
 * Returns 1 if hit, 0 if miss.
 * @param pl    plane geometry
 * @param r     ray to test
 * @param range valid t range (tmin/tmax)
 * @param rec   out hit record to fill on success
 */
int	hit_plane_obj(const t_plane *pl, t_ray *r, t_hit_range range,
	t_hit_record *rec)
{
	double	den;
	double	t;
	t_vec3	outward;

	outward = vec3_normalize(pl->norm);
	den = vec3_dot(outward, r->direction);
	if (fabs(den) < 1e-8)
		return (0);
	t = vec3_dot(vec3_sub(pl->cords, r->origin), outward) / den;
	if (t <= range.tmin || t >= range.tmax)
		return (0);
	rec->t = t;
	rec->p = vec3_add(r->origin, vec3_multiply(r->direction, rec->t));
	compute_plane_tangent_basis(outward, rec);
	compute_plane_uv(rec->p, pl, rec->tangent, rec);
	rec->bump = pl->bump;
	set_face_normal(rec, r, &outward);
	rec->rgb = pl->rgb;
	rec->mat = pl->mat;
	return (1);
}
