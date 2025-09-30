/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_squared.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 14:26:38 by apregitz          #+#    #+#             */
/*   Updated: 2025/09/30 17:14:27 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

inline double	vec3_squared(const t_vec3 *vec)
{
	return (vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);
}
