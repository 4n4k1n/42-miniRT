/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_cpy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 15:26:16 by apregitz          #+#    #+#             */
/*   Updated: 2025/10/26 10:44:45 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Creates a copy of a vector
 * Returns new vector with same values
 * Inline version for performance
 */
inline t_vec3	vec3_cpy(const t_vec3 vec)
{
	return (vec);
}

inline t_vec3	vec3_cpy_ptr(const t_vec3 *vec)
{
	return (*vec);
}
