/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aabb.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 00:00:00 by anakin            #+#    #+#             */
/*   Updated: 2025/10/30 21:02:03 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static bool	calculate_aabb_hit(t_aabb_hit_calc *calc, t_mm mm,
	const t_aabb *box)
{
	int		i;

	i = -1;
	while (++i < 3)
	{
		calc->inv_d = 1.0 / calc->ray_comp[i];
		calc->t0 = (((double *)&box->min)[i] - calc->orig_comp[i])
			* calc->inv_d;
		calc->t1 = (((double *)&box->max)[i] - calc->orig_comp[i])
			* calc->inv_d;
		if (calc->inv_d < 0.0)
		{
			calc->t0 = calc->t0 + calc->t1;
			calc->t1 = calc->t0 - calc->t1;
			calc->t0 = calc->t0 - calc->t1;
		}
		mm.tmin = fmax(calc->t0, mm.tmin);
		mm.tmax = fmin(calc->t1, mm.tmax);
		if (mm.tmax <= mm.tmin)
			return (0);
	}
	return (1);
}

/**
 * Fast ray-AABB intersection test using slab method
 * Returns 1 if ray hits box between tmin and tmax, 0 otherwise
 */
int	aabb_hit(const t_aabb *box, const t_ray *r, t_mm mm)
{
	t_aabb_hit_calc	calc;

	calc.ray_comp[0] = r->direction.x;
	calc.ray_comp[1] = r->direction.y;
	calc.ray_comp[2] = r->direction.z;
	calc.orig_comp[0] = r->origin.x;
	calc.orig_comp[1] = r->origin.y;
	calc.orig_comp[2] = r->origin.z;
	return (calculate_aabb_hit(&calc, mm, box));
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
