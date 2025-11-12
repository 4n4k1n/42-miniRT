/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lambertian.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 16:11:37 by anakin            #+#    #+#             */
/*   Updated: 2025/11/12 12:52:33 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Returns Lambertian texture color (checker if enabled, else white).
 * @param self material
 * @param rec hit record (uses u,v)
 * @return texture RGB
 */
static t_rgb	get_lambertian_texture(const t_material *self,
	const t_hit_record *rec)
{
	double	s;
	int		m;

	if (self->texture_type == CHECKER)
	{
		if (self->texture_scale <= 0.0)
			s = 1.0;
		else
			s = self->texture_scale;
		m = (((int)floor(rec->u * s)) + ((int)floor(rec->v * s))) & 1;
		if (m)
			return (self->texture_b);
		else
			return (self->texture_a);
	}
	return ((t_rgb){255.0, 255.0, 255.0});
}

/**
 * Lambertian scatter: random hemisphere dir, sets ray & attenuation.
 * Applies checker texture if present.
 * @param self material
 * @param ctx scatter context (reads rec, writes scattered/attenuation)
 * @return 1 always
 */
static int	lambertian_scatter(const t_material *self,
	struct s_scatter_ctx *ctx)
{
	t_vec3	dir;
	t_rgb	base;
	t_rgb	texc;

	(void)ctx->r_in;
	dir = random_on_hemisphere((t_vec3 *)&ctx->rec->normal);
	ctx->scattered->origin = apply_surface_bias(ctx->rec->p, dir,
			ctx->rec->normal);
	ctx->scattered->direction = dir;
	texc = get_lambertian_texture(self, ctx->rec);
	base = rgb_modulate(ctx->rec->rgb, texc);
	*ctx->attenuation = rgb_modulate(self->albedo, base);
	return (1);
}

/**
 * Allocates Lambertian material (diffuse).
 * @param albedo base color
 * @return material or NULL
 */
t_material	*material_lambertian(t_rgb albedo)
{
	t_material	*m;

	m = (t_material *)malloc(sizeof(t_material));
	if (!m)
		return (NULL);
	m->scatter = &lambertian_scatter;
	m->albedo = albedo;
	m->fuzz = 0.0;
	m->type = 0;
	m->refraction_index = 1.0;
	m->texture_type = NONE;
	m->texture_a = (t_rgb){255.0, 255.0, 255.0};
	m->texture_b = (t_rgb){0.0, 0.0, 0.0};
	m->texture_scale = 1.0;
	return (m);
}
