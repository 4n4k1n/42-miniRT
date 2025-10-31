/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aabb.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 00:00:00 by anakin            #+#    #+#             */
/*   Updated: 2025/10/28 00:00:00 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Fast ray-AABB intersection test using slab method
 * Returns 1 if ray hits box between tmin and tmax, 0 otherwise
 */
int	aabb_hit(const t_aabb *box, const t_ray *r, double tmin, double tmax)
{
	double	t0;
	double	t1;
	double	inv_d;
	int		i;
	double	ray_comp[3];
	double	orig_comp[3];

	ray_comp[0] = r->direction.x;
	ray_comp[1] = r->direction.y;
	ray_comp[2] = r->direction.z;
	orig_comp[0] = r->origin.x;
	orig_comp[1] = r->origin.y;
	orig_comp[2] = r->origin.z;
	i = -1;
	while (++i < 3)
	{
		inv_d = 1.0 / ray_comp[i];
		t0 = (((double *)&box->min)[i] - orig_comp[i]) * inv_d;
		t1 = (((double *)&box->max)[i] - orig_comp[i]) * inv_d;
		if (inv_d < 0.0)
		{
			t0 = t0 + t1;
			t1 = t0 - t1;
			t0 = t0 - t1;
		}
		tmin = fmax(t0, tmin);
		tmax = fmin(t1, tmax);
		if (tmax <= tmin)
			return (0);
	}
	return (1);
}

/**
 * Expands bounding box to include a point
 */
t_aabb	aabb_expand_point(t_aabb box, t_vec3 point)
{
	box.min.x = fmin(box.min.x, point.x);
	box.min.y = fmin(box.min.y, point.y);
	box.min.z = fmin(box.min.z, point.z);
	box.max.x = fmax(box.max.x, point.x);
	box.max.y = fmax(box.max.y, point.y);
	box.max.z = fmax(box.max.z, point.z);
	return (box);
}

/**
 * Combines two bounding boxes
 */
t_aabb	aabb_union(t_aabb a, t_aabb b)
{
	t_aabb	result;

	result.min.x = fmin(a.min.x, b.min.x);
	result.min.y = fmin(a.min.y, b.min.y);
	result.min.z = fmin(a.min.z, b.min.z);
	result.max.x = fmax(a.max.x, b.max.x);
	result.max.y = fmax(a.max.y, b.max.y);
	result.max.z = fmax(a.max.z, b.max.z);
	return (result);
}

/**
 * Gets the centroid of a bounding box
 */
t_vec3	aabb_centroid(const t_aabb *box)
{
	t_vec3	result;

	result.x = (box->min.x + box->max.x) * 0.5;
	result.y = (box->min.y + box->max.y) * 0.5;
	result.z = (box->min.z + box->max.z) * 0.5;
	return (result);
}
