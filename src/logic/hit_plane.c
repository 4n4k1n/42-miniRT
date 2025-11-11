/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_plane.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 11:32:34 by apregitz          #+#    #+#             */
/*   Updated: 2025/11/10 22:40:00 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

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
 * Tests if a ray intersects with a plane
 * Computes UV and tangent basis at the hit for bump mapping
 * Returns 1 if hit, 0 if miss
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
