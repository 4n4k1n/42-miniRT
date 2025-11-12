/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_add.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:32:23 by apregitz          #+#    #+#             */
/*   Updated: 2025/11/12 16:21:10 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Component-wise addition of two 3D vectors.
 * Computes the sum a + b and returns the resulting vector. Marked inline to
 * encourage the compiler to inline this small hot-path operation.
 * @param a first addend vector
 * @param b second addend vector
 * @return vector containing component-wise sum (a + b)
 */
__attribute__((always_inline)) inline t_vec3	vec3_add(const t_vec3 a,
	const t_vec3 b)
{
	return ((t_vec3){a.x + b.x, a.y + b.y, a.z + b.z});
}
