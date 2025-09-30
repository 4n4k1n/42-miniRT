/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_cross.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 15:48:25 by apregitz          #+#    #+#             */
/*   Updated: 2025/09/30 17:13:29 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

inline t_vec3	vec3_cross_inline(const t_vec3 *a, const t_vec3 *b)
{
	t_vec3	new;

	new.x = a->y * b->z - a->z * b->y;
	new.y = a->z * b->x - a->x * b->z;
	new.z = a->x * b->y - a->y * b->x;
	return (new);
}
