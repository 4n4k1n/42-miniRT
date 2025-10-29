/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_reflect.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apregitz <apregitz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 00:00:00 by anakin            #+#    #+#             */
/*   Updated: 2025/10/29 14:21:33 by apregitz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Reflects vector v across surface normal n
 * Formula: r = v - 2(v·n)n
 */
t_vec3	vec3_reflect(const t_vec3 v, const t_vec3 n)
{
	double	d;
	t_vec3	twice_n;

	d = 2.0 * vec3_dot(v, n);
	twice_n = vec3_multiply(n, d);
	return (vec3_sub(v, twice_n));
}
