/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_hit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 00:00:00 by anakin            #+#    #+#             */
/*   Updated: 2025/10/28 00:00:00 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static int	test_leaf_objects(t_bvh_node *node, t_ray *r,
	t_hit_range range, t_hit_record *rec)
{
	t_hit_record	tmp;
	t_hit_range		cur_range;
	int				hit_any;
	int				i;

	cur_range.tmax = range.tmax;
	hit_any = 0;
	i = 0;
	while (i < node->obj_count)
	{
		cur_range.tmin = range.tmin;
		if (hittable_hit(node->objects[i], r, cur_range, &tmp))
		{
			hit_any = 1;
			cur_range.tmax = tmp.t;
			*rec = tmp;
		}
		i++;
	}
	return (hit_any);
}

static int	process_both_hits(t_hit_record *left_rec, t_hit_record *right_rec,
	t_hit_record *rec)
{
	if (left_rec->t < right_rec->t)
		*rec = *left_rec;
	else
		*rec = *right_rec;
	return (1);
}

static int	process_child_hits(t_bvh_hit_pair *pair, t_hit_record *rec)
{
	if (pair->hit_left && pair->hit_right)
		return (process_both_hits(&pair->left_rec, &pair->right_rec, rec));
	if (pair->hit_left)
		*rec = pair->left_rec;
	else if (pair->hit_right)
		*rec = pair->right_rec;
	return (pair->hit_left || pair->hit_right);
}

/**
 * Recursively traverses BVH tree to find ray intersections
 */
int	bvh_hit(t_bvh_node *node, t_ray *r, t_hit_range range, t_hit_record *rec)
{
	t_bvh_hit_pair	pair;

	if (!node)
		return (0);
	if (!aabb_hit(&node->box, r, range.tmin, range.tmax))
		return (0);
	if (node->obj_count > 0)
		return (test_leaf_objects(node, r, range, rec));
	pair.hit_left = bvh_hit(node->left, r, range, &pair.left_rec);
	pair.hit_right = bvh_hit(node->right, r, range, &pair.right_rec);
	return (process_child_hits(&pair, rec));
}
