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

/**
 * Validates field of view angle for camera
 * @param fov Field of view angle in degrees
 * @return 0 if valid [0,180], 1 if invalid
 */
int	validate_fov(double fov)
{
	if (fov < 0 || fov > 180)
		return (rt_error("FOV must be in range [0, 180]"));
	return (0);
}

/**
 * Checks if all vector components are within specified range
 * @param vec Pointer to 3D vector to check
 * @param min Minimum allowed value
 * @param max Maximum allowed value
 * @return 1 if all components in range, 0 otherwise
 */
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

/**
 * Checks if vector is non-zero (at least one component != 0)
 * @param v 3D vector to check
 * @return 1 if non-zero, 0 if zero vector
 */
int	vec_non_zero(t_vec3 v)
{
	return (!(v.x == 0.0 && v.y == 0.0 && v.z == 0.0));
}

/**
 * Checks if double value is within specified range [min,max]
 * @param v Value to check
 * @param min Minimum allowed value (inclusive)
 * @param max Maximum allowed value (inclusive)
 * @return 1 if in range, 0 otherwise
 */
int	in_range_d(double v, double min, double max)
{
	return (v >= min && v <= max);
}
