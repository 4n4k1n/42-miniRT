/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bounds.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 00:00:00 by anakin            #+#    #+#             */
/*   Updated: 2025/11/11 13:53:13 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static t_vec3	get_perpendicular(t_vec3 n)
{
	t_vec3	tmp;

	if (fabs(n.y) < 0.999)
		tmp = (t_vec3){0.0, 1.0, 0.0};
	else
		tmp = (t_vec3){1.0, 0.0, 0.0};
	return (tmp);
}

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
 * Calculates bounding box for plane
 * Uses large but finite box to avoid infinite bounds
 * Reduced from 1e6 to 100 to improve BVH partitioning
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
