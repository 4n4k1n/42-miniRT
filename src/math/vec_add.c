/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_add.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:32:23 by apregitz          #+#    #+#             */
/*   Updated: 2025/10/26 10:42:35 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Adds two vectors, modifying the destination vector
 * Formula: dst = dst + src for each component
 * Returns pointer to modified destination
 */
inline t_vec3	vec3_add(const t_vec3 a, const t_vec3 b)
{
	return ((t_vec3){a.x + b.x, a.y + b.y, a.z + b.z});
}

inline t_vec3	vec3_add_ptr(const t_vec3 *a, const t_vec3 *b)
{
	return ((t_vec3){a->x + b->x, a->y + b->y, a->z + b->z});
}
