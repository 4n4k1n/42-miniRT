/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bounds_cones.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 15:56:33 by nweber            #+#    #+#             */
/*   Updated: 2025/11/11 16:12:07 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

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
