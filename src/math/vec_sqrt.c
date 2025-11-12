/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_sqrt.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 14:32:12 by apregitz          #+#    #+#             */
/*   Updated: 2025/11/12 16:14:30 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"
#include <math.h>
#define _GNU_SOURCE

double			sqrt(double x);

/**
 * Calculate the Euclidean length (magnitude) of a 3D vector.
 * Uses sqrt on the sum of squared components. Marked always_inline to hint
 * the compiler to inline this small hot-path math function.
 * @param vec input 3D vector
 * @return length (>= 0.0) of the vector
 */
__attribute__((always_inline)) inline double	vec3_sqrt(const t_vec3 vec)
{
	return (sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z));
}
