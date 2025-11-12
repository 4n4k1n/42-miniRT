/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_hit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 00:00:00 by anakin            #+#    #+#             */
/*   Updated: 2025/11/12 13:41:02 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Tests all objects stored in a BVH leaf against the ray and returns the
 * closest hit within the provided range.
 * @param node BVH leaf node containing objects and obj_count
 * @param r ray to cast
 * @param range valid t range (tmin/tmax)
 * @param rec out hit record of the closest hit (if any)
 * @return 1 if any object in the leaf was hit, 0 otherwise
 */
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

/**
 * Chooses the nearer of two hit records (left/right) and writes it to rec.
 * @param left_rec hit record from left child
 * @param right_rec hit record from right child
 * @param rec out chosen (closer) hit record
 * @return always 1 (indicates a hit was selected)
 */
static int	process_both_hits(t_hit_record *left_rec, t_hit_record *right_rec,
	t_hit_record *rec)
{
	if (left_rec->t < right_rec->t)
		*rec = *left_rec;
	else
		*rec = *right_rec;
	return (1);
}

/**
 * Processes the boolean hit results of child traversals and selects the
 * appropriate hit record to return to the caller.
 * @param pair pair structure containing hit flags and child records
 * @param rec out chosen hit record when one or both children hit
 * @return 1 if either child hit, 0 otherwise
 */
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
 * Recursively traverses the BVH to find the closest intersection of ray r
 * with any object contained in the subtree rooted at node.
 * Performs an AABB test to cull branches. For leaf nodes it tests each object;
 * for interior nodes it recurses into children and selects the nearest hit.
 * @param node BVH node to traverse
 * @param r ray to cast
 * @param range valid t range (tmin/tmax)
 * @param rec out hit record for the closest hit (if any)
 * @return 1 if a hit was found in this subtree, 0 otherwise
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
