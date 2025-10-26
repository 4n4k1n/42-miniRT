/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_overload.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 14:07:06 by apregitz          #+#    #+#             */
/*   Updated: 2025/10/26 10:48:29 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Negates all components of a vector, returns new vector
 * Formula: new_vec = -vec for each component
 * Inline version for performance
 */
inline t_vec3	vec3_overload(const t_vec3 a)
{
	return ((t_vec3){-a.x, -a.y, -a.z});
}

inline t_vec3	vec3_overload_ptr(const t_vec3 *a)
{
	return ((t_vec3){-a->x, -a->y, -a->z});
}