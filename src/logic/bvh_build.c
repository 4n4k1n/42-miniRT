/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_build.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 00:00:00 by anakin            #+#    #+#             */
/*   Updated: 2025/10/28 19:58:10 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

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
 * Builds BVH from object list
 * Excludes planes from BVH as they are infinite and should be tested separately
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
