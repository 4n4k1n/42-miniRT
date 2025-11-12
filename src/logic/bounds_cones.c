/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bounds_cones.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 15:56:33 by nweber            #+#    #+#             */
/*   Updated: 2025/11/12 15:23:52 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Computes an axis-aligned bounding box for a finite cone.
 * The cone is centered at co->cords, oriented along co->norm, has total
 * height co->height and base radius co->diameter / 2. The box is computed
 * by projecting the cone's extent onto each axis: a combination of the
 * half-height contribution along the axis direction and the maximal radial
 * extent perpendicular to that axis.
 * @param co pointer to cone geometry (cords, norm, height, diameter)
 * @return axis-aligned bounding box enclosing the cone
 */
t_aabb	get_cone_bounds(const t_cone *co)
{
	t_aabb	box;
	t_vec3	n;
	double	hh;
	double	r;

	n = vec3_normalize(co->norm);
	hh = co->height * 0.5;
	r = co->diameter * 0.5;
	box.min.x = co->cords.x - (hh * fabs(n.x)
			+ r * sqrt(fmax(0.0, 1.0 - n.x * n.x)));
	box.max.x = co->cords.x + (hh * fabs(n.x)
			+ r * sqrt(fmax(0.0, 1.0 - n.x * n.x)));
	box.min.y = co->cords.y - (hh * fabs(n.y)
			+ r * sqrt(fmax(0.0, 1.0 - n.y * n.y)));
	box.max.y = co->cords.y + (hh * fabs(n.y)
			+ r * sqrt(fmax(0.0, 1.0 - n.y * n.y)));
	box.min.z = co->cords.z - (hh * fabs(n.z)
			+ r * sqrt(fmax(0.0, 1.0 - n.z * n.z)));
	box.max.z = co->cords.z + (hh * fabs(n.z)
			+ r * sqrt(fmax(0.0, 1.0 - n.z * n.z)));
	return (box);
}
