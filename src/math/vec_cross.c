/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_cross.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 15:48:25 by apregitz          #+#    #+#             */
/*   Updated: 2025/10/26 10:44:30 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Calculates cross product of two vectors
 * Formula: a×b = (ay*bz-az*by, az*bx-ax*bz, ax*by-ay*bx)
 * Returns vector perpendicular to both input vectors
 */
inline t_vec3	vec3_cross(const t_vec3 a, const t_vec3 b)
{
	return ((t_vec3){a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, \
		a.x * b.y - a.y * b.x});
}
