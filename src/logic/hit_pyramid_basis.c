/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_pyramid_basis.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 00:00:00 by nweber            #+#    #+#             */
/*   Updated: 2025/11/12 13:10:36 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Returns the normalized vector of v, or v unchanged if its length is zero.
 * @param v input vector
 * @return normalized v, or v if zero-length
 */
t_vec3	norm_or_zero(t_vec3 v)
{
	double	len;

	len = sqrt(vec3_dot(v, v));
	if (len == 0.0)
		return (v);
	return (vec3_divide(v, len));
}

/**
 * Builds an orthonormal right/forward basis from the given up vector.
 * Chooses a stable fallback axis to avoid degeneracy when up is near the X axis.
 * @param up input up vector (not required to be unit)
 * @param right out right vector (unit)
 * @param fwd out forward vector (unit)
 */
void	build_basis(t_vec3 up, t_vec3 *right, t_vec3 *fwd)
{
	t_vec3	t;

	if (fabs(up.x) > 0.9)
		t = (t_vec3){0.0, 1.0, 0.0};
	else
		t = (t_vec3){1.0, 0.0, 0.0};
	*right = norm_or_zero(vec3_cross(up, t));
	*fwd = vec3_cross(*right, up);
}

/**
 * Validates barycentric coordinates for a triangle hit.
 * Returns 1 when u and v lie inside the standard triangle domain:
 * u in [0,1], v >= 0 and u+v <= 1.
 * @param uv pointer to structure containing u and v
 * @return 1 valid, 0 invalid
 */
int	check_uv_bounds(t_py_tri_uv *uv)
{
	if (uv->u < 0.0 || uv->u > 1.0)
		return (0);
	if (uv->v < 0.0 || uv->u + uv->v > 1.0)
		return (0);
	return (1);
}
