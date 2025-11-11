/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bounds_dispatch.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 00:00:00 by anakin            #+#    #+#             */
/*   Updated: 2025/11/11 15:58:18 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Gets bounding box for any object type
 */
t_aabb	get_object_bounds(const t_obj *obj)
{
	if (obj->type == SPHERE)
		return (get_sphere_bounds(&obj->data.sphere));
	else if (obj->type == PLANE)
		return (get_plane_bounds(&obj->data.plane));
	else if (obj->type == CYLINDER)
		return (get_cylinder_bounds(&obj->data.cylinder));
	else if (obj->type == TRIANGLE)
		return (get_triangle_bounds(&obj->data.triangle));
	else if (obj->type == CONE)
		return (get_cone_bounds(&obj->data.cone));
	return ((t_aabb){{0, 0, 0}, {0, 0, 0}});
}
