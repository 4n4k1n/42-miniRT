/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_overload.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 14:07:06 by apregitz          #+#    #+#             */
/*   Updated: 2025/11/12 16:16:02 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Negates a 3D vector (component-wise).
 * Returns a new vector with each component multiplied by -1.
 * Marked inline to encourage compiler inlining for hot-path math.
 * @param a input vector
 * @return negated vector (-a)
 */
__attribute__((always_inline)) inline t_vec3	vec3_overload(const t_vec3 a)
{
	return ((t_vec3){-a.x, -a.y, -a.z});
}
