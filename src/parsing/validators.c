/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validators.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 20:27:38 by nweber            #+#    #+#             */
/*   Updated: 2025/09/30 11:32:34 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

int	validate_fov(double fov)
{
	if (fov < 0 || fov > 180)
		return (rt_error("FOV must be in range [0, 180]"));
	return (0);
}

int	vec_in_range(t_vec3 *vec, double min, double max)
{
	if (!vec)
		return (0);
	if (vec->x < min || vec->x > max)
		return (0);
	if (vec->y < min || vec->y > max)
		return (0);
	if (vec->z < min || vec->z > max)
		return (0);
	return (1);
}

int	vec_non_zero(t_vec3 v)
{
	return (!(v.x == 0.0 && v.y == 0.0 && v.z == 0.0));
}

int	in_range_d(double v, double min, double max)
{
	return (v >= min && v <= max);
}
