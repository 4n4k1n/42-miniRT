/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_dot.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 15:40:50 by apregitz          #+#    #+#             */
/*   Updated: 2025/09/30 17:13:45 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Calculates dot product of two vectors
 * Formula: a·b = ax*bx + ay*by + az*bz
 * Returns scalar value used for angles and projections
 */
inline double	vec3_dot_inline(const t_vec3 *a, const t_vec3 *b)
{
	return (a->x * b->x + a->y * b->y + a->z * b->z);
}
