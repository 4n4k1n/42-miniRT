/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   surface_bias.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apregitz <apregitz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 00:00:00 by anakin            #+#    #+#             */
/*   Updated: 2025/10/29 14:28:06 by apregitz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Applies a small bias to surface point to prevent shadow acne
 * Offsets point slightly along normal in direction of the scattered ray
 * Prevents self-intersection artifacts in ray tracing
 */
__attribute__((always_inline)) inline t_vec3	apply_surface_bias(t_vec3 point,
	t_vec3 direction, t_vec3 normal)
{
	double	eps;
	double	sign;
	t_vec3	bias;

	eps = 1e-4;
	if (vec3_dot(direction, normal) > 0.0)
		sign = 1.0;
	else
		sign = -1.0;
	bias = vec3_multiply(normal, eps * sign);
	return (vec3_add(point, bias));
}
