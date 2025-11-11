/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_pyramid_basis.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 00:00:00 by nweber            #+#    #+#             */
/*   Updated: 2025/11/11 00:00:00 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

t_vec3	norm_or_zero(t_vec3 v)
{
	double	len;

	len = sqrt(vec3_dot(v, v));
	if (len == 0.0)
		return (v);
	return (vec3_divide(v, len));
}

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

int	check_uv_bounds(t_py_tri_uv *uv)
{
	if (uv->u < 0.0 || uv->u > 1.0)
		return (0);
	if (uv->v < 0.0 || uv->u + uv->v > 1.0)
		return (0);
	return (1);
}
