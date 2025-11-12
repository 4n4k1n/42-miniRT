/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_zero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 14:19:55 by apregitz          #+#    #+#             */
/*   Updated: 2025/11/12 16:10:07 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Tests whether a 3D vector is (approximately) zero.
 * Uses a small epsilon to account for floating point imprecision. Returns
 * non-zero when all components have absolute value below the threshold.
 * @param v pointer to vector to test
 * @return non-zero if vector is near zero, zero otherwise
 */
int	vec3_near_zero(const t_vec3 *v)
{
	double	s;

	s = 1e-8;
	return ((fabs(v->x) < s) && (fabs(v->y) < s) && (fabs(v->z) < s));
}
