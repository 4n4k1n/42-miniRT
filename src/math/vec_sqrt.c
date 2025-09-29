/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_sqrt.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 14:32:12 by apregitz          #+#    #+#             */
/*   Updated: 2025/09/29 21:03:41 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _GNU_SOURCE
#include "mini_rt.h"
#include <math.h>

double sqrt(double x);

inline double	vec3_sqrt(t_vec3 *vec)
{
	return (sqrt(vec3_squared(vec)));
}
