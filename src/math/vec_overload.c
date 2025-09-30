/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_overload.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 14:07:06 by apregitz          #+#    #+#             */
/*   Updated: 2025/09/30 17:13:17 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

t_vec3	*vec3_overload(t_vec3 *vec)
{
	vec->x *= -1;
	vec->y *= -1;
	vec->z *= -1;
	return (vec);
}

inline t_vec3	vec3_overload_inline(const t_vec3 *vec)
{
	t_vec3	new;

	new.x = vec->x * -1;
	new.y = vec->y * -1;
	new.z = vec->z * -1;
	return (new);
}
