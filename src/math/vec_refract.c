/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_refract.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 13:14:57 by nweber            #+#    #+#             */
/*   Updated: 2025/10/31 13:15:12 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

t_vec3	vec3_refract(const t_vec3 v, const t_vec3 n, double etai_over_etat)
{
	t_vec3	uv;
	double	cos_theta;
	t_vec3	r_out_parallel;

	uv = vec3_normalize(v);
	cos_theta = fmin(vec3_dot(vec3_overload(uv), n), 1.0);
	if (1.0 - etai_over_etat * etai_over_etat
		* (1.0 - cos_theta * cos_theta) > 0.0)
		r_out_parallel = vec3_multiply(n, -sqrt(1.0 - \
			etai_over_etat * etai_over_etat * (1.0 - cos_theta * cos_theta)));
	else
		r_out_parallel = vec3_multiply(n, 0.0);
	return (vec3_add(vec3_multiply(vec3_add(uv, \
		vec3_multiply(n, cos_theta)), etai_over_etat), r_out_parallel));
}
