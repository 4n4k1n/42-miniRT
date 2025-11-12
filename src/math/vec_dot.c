/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_dot.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 15:40:50 by apregitz          #+#    #+#             */
/*   Updated: 2025/11/12 16:19:41 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Compute the dot (scalar) product of two 3D vectors.
 * Formula: a·b = ax*bx + ay*by + az*bz
 * The dot product is used for projections, angle computations and length
 * calculations in the renderer.
 * @param a first vector
 * @param b second vector
 * @return scalar dot product
 */
__attribute__((always_inline)) inline double	vec3_dot(const t_vec3 a,
	const t_vec3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}
