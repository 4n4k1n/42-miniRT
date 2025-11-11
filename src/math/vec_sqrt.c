/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_sqrt.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apregitz <apregitz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 14:32:12 by apregitz          #+#    #+#             */
/*   Updated: 2025/10/29 14:19:36 by apregitz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"
#include <math.h>
#define _GNU_SOURCE

double			sqrt(double x);

/**
 * Calculates the length (magnitude) of a vector
 * Formula: |vec| = √(x² + y² + z²)
 * Returns scalar length of the vector
 */
__attribute__((always_inline)) inline double	vec3_sqrt(const t_vec3 vec)
{
	return (sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z));
}
