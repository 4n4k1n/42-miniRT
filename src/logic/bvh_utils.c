/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 00:00:00 by anakin            #+#    #+#             */
/*   Updated: 2025/10/28 00:00:00 by anakin           ###   ########.fr       */
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
