/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_cpy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 15:26:16 by apregitz          #+#    #+#             */
/*   Updated: 2025/11/12 16:20:58 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Return a copy of the provided vector.
 * This trivial helper returns the input vector by value and is provided to
 * keep vector semantics explicit in call sites. Marked inline for performance.
 * @param vec input vector to copy
 * @return copy of vec
 */
__attribute__((always_inline)) inline t_vec3	vec3_cpy(const t_vec3 vec)
{
	return (vec);
}
