/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_sub.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:40:26 by apregitz          #+#    #+#             */
/*   Updated: 2025/10/26 10:52:29 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Subtracts two vectors, returns new vector
 * Formula: new_vec = dst - src for each component
 * Inline version for performance
 */
__attribute__((always_inline)) inline t_vec3	vec3_sub(const t_vec3 a,
	const t_vec3 b)
{
	return ((t_vec3){a.x - b.x, a.y - b.y, a.z - b.z});
}
