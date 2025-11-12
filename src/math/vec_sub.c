/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_sub.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:40:26 by apregitz          #+#    #+#             */
/*   Updated: 2025/11/12 16:11:37 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Subtract two 3D vectors component-wise.
 * Computes: result = a - b
 * Marked inline and always_inline to encourage the compiler to inline the
 * small hot-path vector math for performance-sensitive code.
 * @param a minuend vector
 * @param b subtrahend vector
 * @return the vector difference (a - b)
 */
__attribute__((always_inline)) inline t_vec3	vec3_sub(const t_vec3 a,
	const t_vec3 b)
{
	return ((t_vec3){a.x - b.x, a.y - b.y, a.z - b.z});
}
