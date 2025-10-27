/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_multiply.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 14:16:31 by apregitz          #+#    #+#             */
/*   Updated: 2025/10/26 10:47:46 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Multiplies vector by scalar, returns new vector
 * Formula: new_vec = vec * t for each component
 * Inline version for performance
 */
inline t_vec3	vec3_multiply(const t_vec3 a, double t)
{
	return ((t_vec3){a.x * t, a.y * t, a.z * t});
}
