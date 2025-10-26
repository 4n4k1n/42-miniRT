/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random_vec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 12:00:52 by apregitz          #+#    #+#             */
/*   Updated: 2025/10/26 15:16:35 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

t_vec3	random_vec3(double min, double max)
{
	t_vec3	vec;

	vec.x = random_double_range(min, max);
	vec.y = random_double_range(min, max);
	vec.z = random_double_range(min, max);
	return (vec);
}

void	randomize_vec3(t_vec3 *vec, double min, double max)
{
	vec->x = random_double_range(min, max);
	vec->y = random_double_range(min, max);
	vec->z = random_double_range(min, max);
}

t_vec3	random_unit_vec3(void)
{
	t_vec3	vec;
	double	lensq;
	
	while (true)
	{
		randomize_vec3(&vec, -1.0, 1.0);
		lensq = vec3_dot(vec, vec);
		if (lensq > 1e-160 && lensq <= 1)
			return (vec3_divide(vec, sqrt(lensq)));
	}
}

t_vec3	random_on_hemisphere(t_vec3 *normal)
{
	t_vec3	on_unit_sphere;

	on_unit_sphere = random_unit_vec3();
	if (vec3_dot_ptr(&on_unit_sphere, normal) > 0.0)
		return (on_unit_sphere);
	return (vec3_overload(on_unit_sphere));
}
