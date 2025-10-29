/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bump.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 14:58:42 by nweber            #+#    #+#             */
/*   Updated: 2025/10/28 15:00:36 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static inline double bump_height(const t_bump *b, double u, double v)
{
	uint32_t	ix;
	uint32_t	iy;
	size_t		idx;
	double		r;
	double		g;
	double		bl;

	if (!b || !b->pixels || b->width == 0 || b->height == 0)
		return (0.0);
	while (u < 0.0)
		u += 1.0;
	while (v < 0.0)
		v += 1.0;
	if (u > 1.0)
		u = u - floor(u);
	if (v > 1.0)
		v = v - floor(v);
	ix = (uint32_t)fmin((double)(b->width - 1), floor(u * (double)b->width));
	iy = (uint32_t)fmin((double)(b->height - 1), floor(v * (double)b->height));
	idx = ((size_t)iy * (size_t)b->width + (size_t)ix) * 4;
	r = (double)b->pixels[idx + 0] / 255.0;
	g = (double)b->pixels[idx + 1] / 255.0;
	bl = (double)b->pixels[idx + 2] / 255.0;
	return 0.2126 * r + 0.7152 * g + 0.0722 * bl;
}

/**
 * Perturbs a normal vector given tangent/bitangent basis and UV
 * Uses forward differences on the height field and scales by bump->strength
 */
t_vec3	bump_perturb_from_uv(const t_bump *b, t_vec3 n, t_vec3 t, t_vec3 bta, double u, double v)
{
	double	h;
	double	hu;
	double	hv;
	double	du;
	double	dv;
	t_vec3	off;

	if (!b)
		return (n);
	du = 1.0 / (double)fmax(1u, b->width);
	dv = 1.0 / (double)fmax(1u, b->height);
	h = bump_height(b, u, v);
	hu = bump_height(b, u + du, v);
	hv = bump_height(b, u, v + dv);
	off = vec3_add(vec3_multiply(t, (hu - h)), vec3_multiply(bta, (hv - h)));
	off = vec3_multiply(off, b->strength);
	return vec3_normalize(vec3_add(n, off));
}
