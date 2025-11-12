/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_squared.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 14:26:38 by apregitz          #+#    #+#             */
/*   Updated: 2025/11/12 16:13:00 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Compute the squared length of a 3D vector.
 * This avoids the cost of sqrt when only relative lengths or comparisons are
 * required. Returns x*x + y*y + z*z.
 * @param vec input vector
 * @return squared magnitude of vec
 */
__attribute__((always_inline)) inline double	vec3_squared(const t_vec3 vec)
{
	return (vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}
