/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_normalize.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 00:00:00 by anakin            #+#    #+#             */
/*   Updated: 2025/11/12 16:18:09 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Normalize a 3D vector to unit length.
 * Computes the Euclidean length and divides the vector by that length.
 * If the vector has zero length the original vector is returned unchanged
 * to avoid division by zero.
 * @param v input vector
 * @return normalized vector (unit length) or original v if length == 0
 */
__attribute__((always_inline)) inline t_vec3	vec3_normalize(const t_vec3 v)
{
	double	len;

	len = vec3_sqrt(v);
	if (len == 0.0)
		return (v);
	return (vec3_divide(v, len));
}
