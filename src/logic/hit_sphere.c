/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_sphere.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 11:32:34 by apregitz          #+#    #+#             */
/*   Updated: 2025/10/29 15:18:44 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Tests if a ray intersects with a sphere object
 * Computes UV and tangent basis at the hit for bump mapping
 * Returns 1 if hit, 0 if miss
 */
int	hit_sphere_obj(const t_sphere *s, t_ray *r, double tmin, double tmax, t_hit_record *rec)
{
	t_vec3	oc;
	double	a;
	double	h;
	double	c;
	double	dis;
	double	root;
	t_vec3	outward;
	t_vec3	hp;
	t_vec3	tmp;
	double	u;
	double	v;

	oc = vec3_sub(r->origin, s->cords);
	a = vec3_dot(r->direction, r->direction);
	h = vec3_dot(r->direction, oc);
	c = vec3_dot(oc, oc) - (s->diameter * 0.5) * (s->diameter * 0.5);
	dis = h * h - a * c;
	if (dis < 0)
		return (0);
	root = (-h - sqrt(dis)) / a;
	if (root <= tmin || root >= tmax)
	{
		root = (-h + sqrt(dis)) / a;
		if (root <= tmin || root >= tmax)
			return (0);
	}
	rec->t = root;
	hp = vec3_add(r->origin, vec3_multiply(r->direction, rec->t));
	rec->p = hp;
	outward = vec3_divide(vec3_sub(hp, s->cords), (s->diameter * 0.5));
	tmp = (fabs(outward.y) < 0.999) ? (t_vec3){0.0, 1.0, 0.0} : (t_vec3){1.0, 0.0, 0.0};
	rec->tangent = vec3_normalize(vec3_cross(tmp, outward));
	rec->bitangent = vec3_cross(outward, rec->tangent);
	u = 0.5 + atan2(outward.z, outward.x) / (2.0 * M_PI);
	v = 0.5 - asin(outward.y) / M_PI;
	rec->u = u;
	rec->v = v;
	rec->bump = s->bump;
	set_face_normal(rec, r, &outward);
	rec->rgb = s->rgb;
	rec->mat = s->mat;
	return (1);
}
