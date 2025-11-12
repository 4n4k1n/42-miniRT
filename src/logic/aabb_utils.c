/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aabb_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 00:00:00 by anakin            #+#    #+#             */
/*   Updated: 2025/11/12 15:27:03 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Expands an axis-aligned bounding box to include a given point.
 * Updates box.min and box.max component-wise to ensure the point lies
 * within the resulting box.
 * @param box initial bounding box (will be modified and returned)
 * @param point point to include in the bounding box
 * @return expanded bounding box containing the input point
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
 * Computes the union of two axis-aligned bounding boxes.
 * The resulting box is the smallest AABB that contains both input boxes.
 * @param a first bounding box
 * @param b second bounding box
 * @return bounding box enclosing both a and b
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
 * Computes the centroid (center) of an axis-aligned bounding box.
 * The centroid is simply the average of min and max on each axis.
 * @param box pointer to the bounding box
 * @return centroid position as a t_vec3
 */
t_vec3	aabb_centroid(const t_aabb *box)
{
	t_vec3	result;

	result.x = (box->min.x + box->max.x) * 0.5;
	result.y = (box->min.y + box->max.y) * 0.5;
	result.z = (box->min.z + box->max.z) * 0.5;
	return (result);
}
