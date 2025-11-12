/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_cross.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 15:48:25 by apregitz          #+#    #+#             */
/*   Updated: 2025/11/12 16:19:14 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Compute the cross product of two 3D vectors.
 * The cross product yields a vector perpendicular to both inputs and is
 * defined as:
 *   a × b = (a.y*b.z - a.z*b.y,
 *            a.z*b.x - a.x*b.z,
 *            a.x*b.y - a.y*b.x)
 * This function is marked inline to encourage inlining in hot math paths.
 * @param a left-hand vector
 * @param b right-hand vector
 * @return cross product vector (a × b)
 */
__attribute__((always_inline)) inline t_vec3	vec3_cross(const t_vec3 a,
	const t_vec3 b)
{
	return ((t_vec3){a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y
		- a.y * b.x});
}
