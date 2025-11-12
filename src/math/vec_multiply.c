/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_multiply.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 14:16:31 by apregitz          #+#    #+#             */
/*   Updated: 2025/11/12 16:17:54 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Scale a 3D vector by a scalar.
 * Performs a component-wise multiplication of vector 'a' with scalar 't'.
 * Returns the resulting vector. Marked inline to allow the compiler to
 * optimize this small hot-path operation.
 * @param a input vector
 * @param t scalar multiplier
 * @return scaled vector (a * t)
 */
__attribute__((always_inline)) inline t_vec3	vec3_multiply(const t_vec3 a,
	double t)
{
	return ((t_vec3){a.x * t, a.y * t, a.z * t});
}
