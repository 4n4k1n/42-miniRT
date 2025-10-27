/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_divide.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 14:19:58 by apregitz          #+#    #+#             */
/*   Updated: 2025/10/26 10:45:35 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Divides vector by scalar, returns new vector
 * Formula: new_vec = vec / t for each component
 * Inline version for performance
 */
inline t_vec3	vec3_divide(const t_vec3 a, const double t)
{
	return ((t_vec3){a.x / t, a.y / t, a.z / t});
}
