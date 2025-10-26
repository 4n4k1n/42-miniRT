/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_sqrt.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 14:32:12 by apregitz          #+#    #+#             */
/*   Updated: 2025/10/26 13:24:49 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _GNU_SOURCE
#include "mini_rt.h"
#include <math.h>

double sqrt(double x);

/**
 * Calculates the length (magnitude) of a vector
 * Formula: |vec| = √(x² + y² + z²)
 * Returns scalar length of the vector
 */
inline double	vec3_sqrt(const t_vec3 vec)
{
	return (sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z));
}

inline double	vec3_sqrt_ptr(const t_vec3 *vec)
{
	return (sqrt(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z));
}
