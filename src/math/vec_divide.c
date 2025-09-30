/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_divide.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 14:19:58 by apregitz          #+#    #+#             */
/*   Updated: 2025/09/30 17:13:08 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Divides vector by scalar, modifying original vector
 * Formula: vec = vec / t for each component
 * Returns pointer to modified vector
 */
t_vec3	*vec3_divide(t_vec3 *vec, double t)
{
	vec->x /= t;
	vec->y /= t;
	vec->z /= t;
	return (vec);
}

/**
 * Divides vector by scalar, returns new vector
 * Formula: new_vec = vec / t for each component
 * Inline version for performance
 */
inline t_vec3	vec3_divide_inline(const t_vec3 *vec, double t)
{
	t_vec3	new;

	new.x = vec->x / t;
	new.y = vec->y / t;
	new.z = vec->z / t;
	return (new);
}
