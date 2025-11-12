/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   metal.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 13:51:50 by anakin            #+#    #+#             */
/*   Updated: 2025/11/12 12:51:37 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Returns metal texture color (checker if enabled, else white).
 * @param self material
 * @param rec hit record (uses u,v)
 * @return texture RGB
 */
static t_rgb	get_metal_texture(const t_material *self,
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
 * Computes reflected + fuzzed scatter direction for metal.
 * @param dir output scatter direction
 * @param self material (uses fuzz)
 * @param r_in incoming ray
 * @param rec hit info (normal)
 */
static void	compute_metal_scatter(t_vec3 *dir, const t_material *self,
	const t_ray *r_in, const t_hit_record *rec)
{
	t_vec3	reflected;

	reflected = vec3_reflect(vec3_normalize(r_in->direction), rec->normal);
	*dir = vec3_add(reflected, vec3_multiply(random_unit_vec3(), self->fuzz));
}

/**
 * Performs metal scattering: sets scattered ray and attenuation.
 * Returns 1 if the scattered ray goes outward.
 * @param self material
 * @param ctx scatter context
 * @return 1 continue, 0 absorb
 */
static int	process_metal(const t_material *self, struct s_scatter_ctx *ctx)
{
	t_vec3	dir;
	t_rgb	base;
	t_rgb	texc;

	compute_metal_scatter(&dir, self, ctx->r_in, ctx->rec);
	ctx->scattered->origin = apply_surface_bias(ctx->rec->p, dir,
			ctx->rec->normal);
	ctx->scattered->direction = dir;
	texc = get_metal_texture(self, ctx->rec);
	base = rgb_modulate(self->albedo, ctx->rec->rgb);
	*ctx->attenuation = rgb_modulate(base, texc);
	return (vec3_dot(ctx->scattered->direction, ctx->rec->normal) > 0.0);
}

/**
 * Wrapper scatter function for metal.
 * @param self material
 * @param ctx scatter context
 * @return process_metal result
 */
static int	metal_scatter(const t_material *self, struct s_scatter_ctx *ctx)
{
	return (process_metal(self, ctx));
}

/**
 * Allocates a metal material with fuzz clamped to [0,1].
 * @param albedo base color
 * @param fuzz roughness factor
 * @return material pointer or NULL
 */
t_material	*material_metal(t_rgb albedo, double fuzz)
{
	t_material	*m;

	m = (t_material *)malloc(sizeof(t_material));
	if (!m)
		return (NULL);
	if (fuzz < 0.0)
		fuzz = 0.0;
	if (fuzz > 1.0)
		fuzz = 1.0;
	m->scatter = &metal_scatter;
	m->albedo = albedo;
	m->fuzz = fuzz;
	m->type = 1;
	m->refraction_index = 1.0;
	m->texture_type = NONE;
	m->texture_a = (t_rgb){255.0, 255.0, 255.0};
	m->texture_b = (t_rgb){0.0, 0.0, 0.0};
	m->texture_scale = 1.0;
	return (m);
}
