/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_hit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 00:00:00 by anakin            #+#    #+#             */
/*   Updated: 2025/10/30 20:54:55 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static int	test_leaf_objects(t_bvh_node *node, t_ray *r, double tmin,
		double tmax, t_hit_record *rec)
{
	t_hit_record	tmp;
	double			closest;
	int				hit_any;
	int				i;

	closest = tmax;
	hit_any = 0;
	i = 0;
	while (i < node->obj_count)
	{
		if (hittable_hit(node->objects[i], r, tmin, closest, &tmp))
		{
			hit_any = 1;
			closest = tmp.t;
			*rec = tmp;
		}
		i++;
	}
	return (hit_any);
}

/**
 * Recursively traverses BVH tree to find ray intersections
 */
int	bvh_hit(t_bvh_node *node, t_ray *r, double tmin, double tmax,
		t_hit_record *rec)
{
	t_hit_record	left_rec;
	t_hit_record	right_rec;
	int				hit_left;
	int				hit_right;

	if (!node)
		return (0);
	if (!aabb_hit(&node->box, r, (t_mm){tmin, tmax}))
		return (0);
	if (node->obj_count > 0)
		return (test_leaf_objects(node, r, tmin, tmax, rec));
	hit_left = bvh_hit(node->left, r, tmin, tmax, &left_rec);
	hit_right = bvh_hit(node->right, r, tmin, tmax, &right_rec);
	if (hit_left && hit_right)
	{
		if (left_rec.t < right_rec.t)
			*rec = left_rec;
		else
			*rec = right_rec;
		return (1);
	}
	if (hit_left)
		*rec = left_rec;
	else if (hit_right)
		*rec = right_rec;
	return (hit_left || hit_right);
}
