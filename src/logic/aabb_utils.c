/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aabb_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 00:00:00 by anakin            #+#    #+#             */
/*   Updated: 2025/10/28 00:00:00 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

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
