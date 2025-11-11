/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_sphere.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 11:32:34 by apregitz          #+#    #+#             */
/*   Updated: 2025/10/31 11:27:55 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static int	solve_sphere_quad(t_sphere_hit *sh, t_ray *r, double *root)
{
	double	quad[3];
	double	sqrtd;

	quad[0] = vec3_dot(r->direction, r->direction);
	quad[1] = vec3_dot(r->direction, sh->oc);
	quad[2] = vec3_dot(sh->oc, sh->oc) - sh->radius * sh->radius;
	quad[2] = quad[1] * quad[1] - quad[0] * quad[2];
	if (quad[2] < 0)
		return (0);
	sqrtd = sqrt(quad[2]);
	*root = (-quad[1] - sqrtd) / quad[0];
	if (*root <= sh->range.tmin || *root >= sh->range.tmax)
	{
		*root = (-quad[1] + sqrtd) / quad[0];
		if (*root <= sh->range.tmin || *root >= sh->range.tmax)
			return (0);
	}
	return (1);
}

static void	compute_sphere_uv(t_vec3 outward, t_hit_record *rec)
{
	rec->u = 0.5 + atan2(outward.z, outward.x) / (2.0 * M_PI);
	rec->v = 0.5 - asin(outward.y) / M_PI;
}

static void	compute_tangent_basis(t_vec3 outward, t_hit_record *rec)
{
	t_vec3	tmp;

	if (fabs(outward.y) < 0.999)
		tmp = (t_vec3){0.0, 1.0, 0.0};
	else
		tmp = (t_vec3){1.0, 0.0, 0.0};
	rec->tangent = vec3_normalize(vec3_cross(tmp, outward));
	rec->bitangent = vec3_cross(outward, rec->tangent);
}

/**
 * Tests if a ray intersects with a sphere object
 * Computes UV and tangent basis at the hit for bump mapping
 * Returns 1 if hit, 0 if miss
 */
int	hit_sphere_obj(const t_sphere *s, t_ray *r, t_hit_range range,
	t_hit_record *rec)
{
	t_sphere_hit	sh;
	double			root;
	t_vec3			outward;

	sh.oc = vec3_sub(r->origin, s->cords);
	sh.radius = s->diameter * 0.5;
	sh.range = range;
	if (!solve_sphere_quad(&sh, r, &root))
		return (0);
	rec->t = root;
	rec->p = vec3_add(r->origin, vec3_multiply(r->direction, rec->t));
	outward = vec3_divide(vec3_sub(rec->p, s->cords), sh.radius);
	compute_tangent_basis(outward, rec);
	compute_sphere_uv(outward, rec);
	rec->bump = s->bump;
	set_face_normal(rec, r, &outward);
	rec->rgb = s->rgb;
	rec->mat = s->mat;
	return (1);
}
