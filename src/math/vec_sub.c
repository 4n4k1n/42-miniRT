/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_sub.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:40:26 by apregitz          #+#    #+#             */
/*   Updated: 2025/09/30 17:12:44 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Subtracts two vectors, modifying the destination vector
 * Formula: dst = dst - src for each component
 * Returns pointer to modified destination
 */
t_vec3	*vec3_sub(t_vec3 *dst, t_vec3 *src)
{
	dst->x -= src->x;
	dst->y -= src->y;
	dst->z -= src->z;
	return (dst);
}

/**
 * Subtracts two vectors, returns new vector
 * Formula: new_vec = dst - src for each component
 * Inline version for performance
 */
inline t_vec3	vec3_sub_inline(const t_vec3 *dst, const t_vec3 *src)
{
	t_vec3	new;

	new.x = dst->x - src->x;
	new.y = dst->y - src->y;
	new.z = dst->z - src->z;
	return (new);
}
