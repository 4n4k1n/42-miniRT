/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 18:34:21 by apregitz          #+#    #+#             */
/*   Updated: 2025/10/26 10:30:32 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Creates and initializes a new 3D vector
 * Sets x, y, z components to given values
 * Returns new vector by value
 */
inline t_vec3	vec3_init(const double x, const double y, const double z)
{
	return ((t_vec3){x, y, z});
}
