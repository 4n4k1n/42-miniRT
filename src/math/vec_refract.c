/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_refract.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 13:14:57 by nweber            #+#    #+#             */
/*   Updated: 2025/11/12 16:16:27 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Refracts a vector v through a surface with normal n using Snell's law.
 * Computes the refracted direction for an incident vector v and surface
 * normal n using the ratio etai_over_etat. The function expects v to be
 * an arbitrary vector; it normalizes v internally. If total internal
 * reflection would occur the parallel component is clamped to zero.
 * Formula implemented:
 *   uv = normalize(v)
 *   cos_theta = min(dot(-uv, n), 1.0)
 *   r_out_parallel = -sqrt(max(0, 1 - (etai_over_etat)^2 * (1 - cos^2))) * n
 *   r_out_perp = etai_over_etat * (uv + cos_theta * n)
 *   result = r_out_perp + r_out_parallel
 * @param v incident vector
 * @param n surface normal (should be normalized)
 * @param etai_over_etat ratio of refractive indices (n1 / n2)
 * @return refracted direction (unit length when valid)
 */
__attribute__((always_inline)) inline t_vec3	vec3_refract(const t_vec3 v,
	const t_vec3 n, double etai_over_etat)
{
	t_vec3	uv;
	double	cos_theta;
	t_vec3	r_out_parallel;

	uv = vec3_normalize(v);
	cos_theta = fmin(vec3_dot(vec3_overload(uv), n), 1.0);
	if (1.0 - etai_over_etat * etai_over_etat
		* (1.0 - cos_theta * cos_theta) > 0.0)
		r_out_parallel = vec3_multiply(n, -sqrt(1.0 - etai_over_etat
					* etai_over_etat * (1.0 - cos_theta * cos_theta)));
	else
		r_out_parallel = vec3_multiply(n, 0.0);
	return (vec3_add(vec3_multiply(vec3_add(uv, vec3_multiply(n,
						cos_theta)), etai_over_etat), r_out_parallel));
}
