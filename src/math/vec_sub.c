/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_sub.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:40:26 by apregitz          #+#    #+#             */
/*   Updated: 2025/09/30 17:12:44 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

t_vec3	*vec3_sub(t_vec3 *dst, t_vec3 *src)
{
	dst->x -= src->x;
	dst->y -= src->y;
	dst->z -= src->z;
	return (dst);
}

inline t_vec3	vec3_sub_inline(const t_vec3 *dst, const t_vec3 *src)
{
	t_vec3	new;

	new.x = dst->x - src->x;
	new.y = dst->y - src->y;
	new.z = dst->z - src->z;
	return (new);
}
