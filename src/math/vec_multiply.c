/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_multiply.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apregitz <apregitz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 14:16:31 by apregitz          #+#    #+#             */
/*   Updated: 2025/09/29 15:31:47 by apregitz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

t_vec3	*vec3_multiply(t_vec3 *vec, double t)
{
	vec->x *= t;
	vec->y *= t;
	vec->z *= t;
	return (vec);
}

inline t_vec3	vec3_multiply_inline(t_vec3 *vec, double t)
{
	t_vec3	new;

	new.x = vec->x * t;
	new.y = vec->y * t;
	new.z = vec->z * t;
	return (new);
}
