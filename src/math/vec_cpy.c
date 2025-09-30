/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_cpy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apregitz <apregitz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 15:26:16 by apregitz          #+#    #+#             */
/*   Updated: 2025/09/29 15:30:38 by apregitz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

t_vec3	*vec3_cpy(t_vec3 *dst, t_vec3 *src)
{
	dst->x = src->x;
	dst->x = src->y;
	dst->z = src->z;
	return (dst);
}

inline t_vec3	vec3_cpy_inline(t_vec3 *vec)
{
	return (*vec);
}
