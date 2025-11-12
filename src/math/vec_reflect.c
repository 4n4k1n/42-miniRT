/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_reflect.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 00:00:00 by anakin            #+#    #+#             */
/*   Updated: 2025/11/12 16:16:11 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Reflects vector v around normal n.
 * Implements the reflection formula r = v - 2 * (v·n) * n.
 * The returned vector points in the mirror direction of v relative to n.
 * Inline for performance in shading/geometry code.
 * @param v incident vector
 * @param n surface normal (should be normalized)
 * @return reflected vector
 */
__attribute__((always_inline)) inline t_vec3	vec3_reflect(const t_vec3 v,
	const t_vec3 n)
{
	double	d;
	t_vec3	twice_n;

	d = 2.0 * vec3_dot(v, n);
	twice_n = vec3_multiply(n, d);
	return (vec3_sub(v, twice_n));
}
