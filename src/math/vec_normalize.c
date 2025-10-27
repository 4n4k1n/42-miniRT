/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_normalize.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 00:00:00 by anakin            #+#    #+#             */
/*   Updated: 2025/10/27 00:00:00 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Normalizes a vector (converts to unit vector)
 * Formula: normalized = vec / |vec|
 * Returns original vector if length is zero
 */
inline t_vec3	vec3_normalize(const t_vec3 v)
{
	double	len;

	len = vec3_sqrt(v);
	if (len == 0.0)
		return (v);
	return (vec3_divide(v, len));
}
