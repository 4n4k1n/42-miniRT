/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_add.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apregitz <apregitz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:32:23 by apregitz          #+#    #+#             */
/*   Updated: 2025/09/29 15:02:41 by apregitz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

t_vec3	*vec3_add(t_vec3 *dst, t_vec3 *src)
{
	dst->x += src->x;
	dst->y += src->y;
	dst->z += src->z;
	return (dst);
}

inline t_vec3	vec3_add_inline(t_vec3 *dst, t_vec3 *src)
{
	t_vec3	new;

	new.x = dst->x + src->x;
	new.y = dst->y + src->y;
	new.z = dst->z + src->z;
	return (new);
}
