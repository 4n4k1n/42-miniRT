/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_build.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 00:00:00 by anakin            #+#    #+#             */
/*   Updated: 2025/11/12 13:42:42 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Initializes a leaf BVH node by copying the provided object pointers into
 * the node, setting child pointers to NULL and computing the node bounding box.
 * @param node destination leaf node (must have objects array allocated)
 * @param objects array of object pointers to store in the leaf
 * @param count number of objects
 */
static void	init_leaf_node(t_bvh_node *node, t_obj **objects, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		node->objects[i] = objects[i];
		i++;
	}
	node->obj_count = count;
	node->left = NULL;
	node->right = NULL;
	node->box = compute_bounds(objects, count);
}

/**
 * Allocates and returns a BVH leaf node containing a copy of the supplied
 * object pointer array. Returns NULL on allocation failure.
 * @param objects array of object pointers
 * @param count number of objects
 * @return newly allocated leaf node or NULL
 */
t_bvh_node	*create_leaf(t_obj **objects, int count)
{
	t_bvh_node	*node;

	node = malloc(sizeof(t_bvh_node));
	if (!node)
		return (NULL);
	node->objects = malloc(sizeof(t_obj *) * count);
	if (!node->objects)
	{
		free(node);
		return (NULL);
	}
	init_leaf_node(node, objects, count);
	return (node);
}

/**
 * Recursively builds a BVH from an array of object pointers.
 * Splits along the longest axis using centroid partitioning until leaf size.
 * @param objects array of object pointers (will be partially rearranged)
 * @param count number of objects
 * @return root node of built subtree or NULL on allocation failure
 */
static t_bvh_node	*build_bvh_recursive(t_obj **objects, int count)
{
	t_bvh_node	*node;
	t_aabb		box;
	int			axis;
	int			mid;

	if (count <= 4)
		return (create_leaf(objects, count));
	box = compute_bounds(objects, count);
	axis = get_longest_axis(&box);
	mid = partition_objects(objects, count, axis);
	if (mid == 0 || mid == count)
		mid = count / 2;
	node = malloc(sizeof(t_bvh_node));
	if (!node)
		return (NULL);
	node->box = box;
	node->objects = NULL;
	node->obj_count = 0;
	node->left = build_bvh_recursive(objects, mid);
	node->right = build_bvh_recursive(objects + mid, count - mid);
	return (node);
}

/**
 * Collects non-plane objects from the linked object list into the provided
 * array. Planes are excluded because they are handled separately at runtime.
 * @param list source object list
 * @param objects destination array (must have capacity >= list->size)
 * @return number of objects written into objects[]
 */
static int	collect_non_plane_objects(t_obj_list *list, t_obj **objects)
{
	t_obj	*cur;
	int		i;

	cur = list->head;
	i = 0;
	while (cur)
	{
		if (cur->type != PLANE)
			objects[i++] = cur;
		cur = cur->next;
	}
	return (i);
}

/**
 * Builds a BVH for the provided object list.
 * Allocates a temporary array for non-plane objects, builds the tree and
 * returns the root. Returns NULL on OOM or when no buildable objects exist.
 * @param list linked list of scene objects
 * @return root BVH node or NULL
 */
t_bvh_node	*build_bvh(t_obj_list *list)
{
	t_obj		**objects;
	int			count;
	t_bvh_node	*root;

	if (!list || list->size == 0)
		return (NULL);
	objects = malloc(sizeof(t_obj *) * list->size);
	if (!objects)
		return (NULL);
	count = collect_non_plane_objects(list, objects);
	if (count == 0)
	{
		free(objects);
		return (NULL);
	}
	root = build_bvh_recursive(objects, count);
	free(objects);
	return (root);
}
