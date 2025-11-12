/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_divide.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 14:19:58 by apregitz          #+#    #+#             */
/*   Updated: 2025/11/12 16:19:29 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Divide a 3D vector by a scalar.
 * Performs component-wise division: result = a / t.
 * Caller must ensure t != 0. Marked inline for performance in vector math.
 * @param a input vector
 * @param t scalar divisor
 * @return resulting vector after division
 */
__attribute__((always_inline)) inline t_vec3	vec3_divide(const t_vec3 a,
	const double t)
{
	return ((t_vec3){a.x / t, a.y / t, a.z / t});
}
