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

int	get_longest_axis(t_aabb *box)
{
	double	dx;
	double	dy;
	double	dz;

	dx = box->max.x - box->min.x;
	dy = box->max.y - box->min.y;
	dz = box->max.z - box->min.z;
	if (dx > dy && dx > dz)
		return (0);
	else if (dy > dz)
		return (1);
	return (2);
}

double	get_centroid_component(t_obj *obj, int axis)
{
	t_aabb	box;
	t_vec3	centroid;

	box = get_object_bounds(obj);
	centroid = aabb_centroid(&box);
	if (axis == 0)
		return (centroid.x);
	else if (axis == 1)
		return (centroid.y);
	return (centroid.z);
}

void	swap_objects(t_obj **a, t_obj **b)
{
	t_obj	*temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

int	partition_objects(t_obj **objects, int count, int axis)
{
	double	pivot;
	int		i;
	int		j;

	pivot = get_centroid_component(objects[count / 2], axis);
	i = 0;
	j = count - 1;
	while (1)
	{
		while (i < count && get_centroid_component(objects[i], axis) < pivot)
			i++;
		while (j >= 0 && get_centroid_component(objects[j], axis) > pivot)
			j--;
		if (i >= j)
			return (j + 1);
		swap_objects(&objects[i], &objects[j]);
		i++;
		j--;
	}
}

t_aabb	compute_bounds(t_obj **objects, int count)
{
	t_aabb	box;
	t_aabb	obj_box;
	int		i;

	box.min = vec3_init(INFINITY, INFINITY, INFINITY);
	box.max = vec3_init(-INFINITY, -INFINITY, -INFINITY);
	i = 0;
	while (i < count)
	{
		obj_box = get_object_bounds(objects[i]);
		box = aabb_union(box, obj_box);
		i++;
	}
	return (box);
}

t_bvh_node	*create_leaf(t_obj **objects, int count)
{
	t_bvh_node	*node;
	int			i;

	node = malloc(sizeof(t_bvh_node));
	if (!node)
		return (NULL);
	node->objects = malloc(sizeof(t_obj *) * count);
	if (!node->objects)
	{
		free(node);
		return (NULL);
	}
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

/**
 * Builds BVH from object list
 */
t_bvh_node	*build_bvh(t_obj_list *list)
{
	t_obj		**objects;
	t_obj		*cur;
	int			i;
	t_bvh_node	*root;

	if (!list || list->size == 0)
		return (NULL);
	objects = malloc(sizeof(t_obj *) * list->size);
	if (!objects)
		return (NULL);
	cur = list->head;
	i = 0;
	while (cur)
	{
		objects[i++] = cur;
		cur = cur->next;
	}
	root = build_bvh_recursive(objects, list->size);
	free(objects);
	return (root);
}
