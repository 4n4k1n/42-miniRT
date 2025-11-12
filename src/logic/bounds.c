/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bounds.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 00:00:00 by anakin            #+#    #+#             */
/*   Updated: 2025/11/12 15:24:30 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Returns a stable perpendicular vector to n for basis construction.
 * Chooses world-up (0,1,0) unless the input normal is nearly vertical,
 * in which case returns (1,0,0) to avoid a near-zero cross product.
 * @param n input normal (assumed normalized or arbitrary)
 * @return fallback perpendicular vector
 */
static t_vec3	get_perpendicular(t_vec3 n)
{
	t_vec3	tmp;

	if (fabs(n.y) < 0.999)
		tmp = (t_vec3){0.0, 1.0, 0.0};
	else
		tmp = (t_vec3){1.0, 0.0, 0.0};
	return (tmp);
}

/**
 * Computes the four corner positions of a square patch on the plane.
 * The patch is centered at pb->c with tangent pb->t and bitangent pb->b.
 * pb->hs is the half-size of the square. Corners are written into the
 * provided array in counter-clockwise order.
 * @param pb  plane basis containing center (c),
 * 			tangent (t), bitangent (b) and half-size (hs)
 * @param corners out array of 4 corner positions
 */
static void	compute_plane_corners(t_plane_basis *pb, t_vec3 corners[4])
{
	corners[0] = vec3_add(pb->c, vec3_add(vec3_multiply(pb->t, pb->hs),
				vec3_multiply(pb->b, pb->hs)));
	corners[1] = vec3_add(pb->c, vec3_sub(vec3_multiply(pb->t, pb->hs),
				vec3_multiply(pb->b, pb->hs)));
	corners[2] = vec3_sub(pb->c, vec3_sub(vec3_multiply(pb->t, pb->hs),
				vec3_multiply(pb->b, pb->hs)));
	corners[3] = vec3_sub(pb->c, vec3_add(vec3_multiply(pb->t, pb->hs),
				vec3_multiply(pb->b, pb->hs)));
}

/**
 * Builds an axis-aligned bounding box that encloses the given quad corners.
 * Expands the computed extents by eps in all directions to avoid degenerate
 * zero-volume boxes.
 * @param corners array of 4 corner positions
 * @param eps small expansion amount applied to min/max
 * @return computed axis-aligned bounding box
 */
static t_aabb	bounds_from_corners(t_vec3 corners[4], double eps)
{
	t_aabb	box;
	t_vec3	expand;

	box.min.x = fmin(fmin(corners[0].x, corners[1].x),
			fmin(corners[2].x, corners[3].x));
	box.min.y = fmin(fmin(corners[0].y, corners[1].y),
			fmin(corners[2].y, corners[3].y));
	box.min.z = fmin(fmin(corners[0].z, corners[1].z),
			fmin(corners[2].z, corners[3].z));
	box.max.x = fmax(fmax(corners[0].x, corners[1].x),
			fmax(corners[2].x, corners[3].x));
	box.max.y = fmax(fmax(corners[0].y, corners[1].y),
			fmax(corners[2].y, corners[3].y));
	box.max.z = fmax(fmax(corners[0].z, corners[1].z),
			fmax(corners[2].z, corners[3].z));
	expand = vec3_init(eps, eps, eps);
	box.min = vec3_sub(box.min, expand);
	box.max = vec3_add(box.max, expand);
	return (box);
}

/**
 * Calculates bounding box for plane.
 * Since planes are conceptually infinite and break BVH partitioning, this
 * function returns a large but finite square patch centered on the plane
 * position and returns its axis-aligned bounds.
 * @param p plane geometry (uses cords and norm)
 * @return axis-aligned bounding box approximating the plane extent
 */
t_aabb	get_plane_bounds(const t_plane *p)
{
	t_vec3			n;
	t_plane_basis	pb;
	t_vec3			corners[4];

	n = vec3_normalize(p->norm);
	pb.t = vec3_normalize(vec3_cross(get_perpendicular(n), n));
	pb.b = vec3_cross(n, pb.t);
	pb.c = p->cords;
	pb.hs = 10000.0;
	compute_plane_corners(&pb, corners);
	return (bounds_from_corners(corners, 1.0));
}
