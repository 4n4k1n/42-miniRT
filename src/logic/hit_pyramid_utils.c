/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_pyramid_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 00:00:00 by nweber            #+#    #+#             */
/*   Updated: 2025/11/12 13:09:33 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Computes the four base corner positions for a pyramid base.
 * Uses basis->bc as base center, basis->r and basis->f as orthogonal axes
 * and basis->hw as half-width to build the four corner vertices.
 * @param basis precomputed pyramid basis (r, f, bc, hw)
 * @param v     out structure to receive b0..b3
 */
void	compute_base_corners(t_py_basis *basis, t_py_verts *v)
{
	v->b0 = vec3_add(vec3_add(basis->bc, vec3_multiply(basis->r, basis->hw)),
			vec3_multiply(basis->f, basis->hw));
	v->b1 = vec3_add(vec3_sub(basis->bc, vec3_multiply(basis->r, basis->hw)),
			vec3_multiply(basis->f, basis->hw));
	v->b2 = vec3_sub(vec3_sub(basis->bc, vec3_multiply(basis->r, basis->hw)),
			vec3_multiply(basis->f, basis->hw));
	v->b3 = vec3_add(vec3_sub(basis->bc, vec3_multiply(basis->r, basis->hw)),
			vec3_multiply(basis->f, -basis->hw));
}

/**
 * Computes planar UV coordinates for a pyramid hit point.
 * Projects the hit displacement onto the tangent/bitangent basis and wraps
 * into [0,1) by subtracting floor. Useful for texturing the pyramid faces.
 * @param py  pyramid geometry (used for center)
 * @param rec hit record containing p, tangent and bitangent; writes u/v
 */
void	compute_uv(const t_pyramid *py, t_hit_record *rec)
{
	t_vec3	d;
	double	u;
	double	v;

	d = vec3_sub(rec->p, py->cords);
	u = d.x * rec->tangent.x + d.y * rec->tangent.y + d.z * rec->tangent.z;
	v = d.x * rec->bitangent.x + d.y * rec->bitangent.y
		+ d.z * rec->bitangent.z;
	rec->u = u - floor(u);
	rec->v = v - floor(v);
}

/**
 * Builds an orthonormal tangent / bitangent basis from the surface normal.
 * Chooses a stable fallback axis to avoid degeneracy when the normal is
 * nearly aligned with the world up vector.
 * @param rec hit record to update (reads normal, writes tangent/bitangent)
 */
void	set_tangent_basis(t_hit_record *rec)
{
	t_vec3	tmp;

	if (fabs(rec->normal.y) < 0.999)
		tmp = (t_vec3){0.0, 1.0, 0.0};
	else
		tmp = (t_vec3){1.0, 0.0, 0.0};
	rec->tangent = vec3_normalize(vec3_cross(tmp, rec->normal));
	rec->bitangent = vec3_cross(rec->normal, rec->tangent);
}
