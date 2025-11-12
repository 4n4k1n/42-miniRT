/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 18:34:21 by apregitz          #+#    #+#             */
/*   Updated: 2025/11/12 16:17:41 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Construct a 3D vector from scalar components.
 * Returns a t_vec3 initialized with the provided x, y and z values.
 * This helper centralizes vector creation and keeps call sites compact.
 * @param x X component
 * @param y Y component
 * @param z Z component
 * @return newly constructed t_vec3
 */
__attribute__((always_inline)) inline t_vec3	vec3_init(const double x,
	const double y, const double z)
{
	return ((t_vec3){x, y, z});
}
