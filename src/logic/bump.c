/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bump.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 14:58:42 by nweber            #+#    #+#             */
/*   Updated: 2025/11/12 13:44:01 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Normalizes texture coordinates into the [0,1) range using wrapping.
 * @param u pointer to horizontal coordinate (in/out)
 * @param v pointer to vertical coordinate (in/out)
 */
static void	normalize_uv(double *u, double *v)
{
	while (*u < 0.0)
		*u += 1.0;
	while (*v < 0.0)
		*v += 1.0;
	if (*u > 1.0)
		*u = *u - floor(*u);
	if (*v > 1.0)
		*v = *v - floor(*v);
}

/**
 * Computes luminance (luma) from an interleaved RGBA pixel array at idx.
 * Expects pixels in RGBA order and returns luma in [0,1].
 * @param pixels raw pixel buffer (4 bytes per pixel)
 * @param idx byte index of the pixel's R component
 * @return luma value [0,1]
 */
static double	calc_luma(unsigned char *pixels, size_t idx)
{
	double	r;
	double	g;
	double	bl;

	r = (double)pixels[idx + 0] / 255.0;
	g = (double)pixels[idx + 1] / 255.0;
	bl = (double)pixels[idx + 2] / 255.0;
	return (0.2126 * r + 0.7152 * g + 0.0722 * bl);
}

/**
 * Samples the bump map and returns the height (luma) at UV using nearest texel.
 * Safely handles missing bump data by returning 0.0.
 * @param b bump map descriptor (pixels, width, height)
 * @param u horizontal texture coordinate
 * @param v vertical texture coordinate
 * @return height value in [0,1]
 */
static inline double	bump_height(const t_bump *b, double u, double v)
{
	uint32_t	ix;
	uint32_t	iy;
	size_t		idx;

	if (!b || !b->pixels || b->width == 0 || b->height == 0)
		return (0.0);
	normalize_uv(&u, &v);
	ix = (uint32_t)fmin((double)(b->width - 1), floor(u * (double)b->width));
	iy = (uint32_t)fmin((double)(b->height - 1), floor(v * (double)b->height));
	idx = ((size_t)iy * (size_t)b->width + (size_t)ix) * 4;
	return (calc_luma(b->pixels, idx));
}

/**
 * Perturbs a surface normal using the provided tangent/bitangent basis and UV.
 * Uses central differences on the bump map to approximate partial derivatives,
 * scales by bump->strength (amplified by a factor for visual clarity) and
 * returns a normalized perturbed normal. If no bump map is present the
 * original normal is returned unchanged.
 * @param ctx context containing bump map pointer (b), original normal (n),
 *            tangent (t), bitangent (bta)
 * @param u   texture u coordinate
 * @param v   texture v coordinate
 * @return perturbed unit normal
 */
t_vec3	bump_perturb_from_uv(t_bump_ctx *ctx, double u, double v)
{
	double	du;
	double	dv;
	double	hu;
	double	hv;
	t_vec3	off;

	if (!ctx->b)
		return (ctx->n);
	du = 1.0 / (double)fmax(1u, ctx->b->width);
	dv = 1.0 / (double)fmax(1u, ctx->b->height);
	hu = bump_height(ctx->b, u + du, v) - bump_height(ctx->b, u - du, v);
	hv = bump_height(ctx->b, u, v + dv) - bump_height(ctx->b, u, v - dv);
	off = vec3_add(vec3_multiply(ctx->t, hu), vec3_multiply(ctx->bta, hv));
	off = vec3_multiply(off, ctx->b->strength * 6.0);
	return (vec3_normalize(vec3_add(ctx->n, off)));
}
