/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_sqrt.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 14:32:12 by apregitz          #+#    #+#             */
/*   Updated: 2025/09/30 17:14:38 by nweber           ###   ########.fr       */
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
inline double	vec3_sqrt(const t_vec3 *vec)
{
	return (sqrt(vec3_squared(vec)));
}
