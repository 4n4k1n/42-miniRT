/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phong.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 00:00:00 by anakin            #+#    #+#             */
/*   Updated: 2025/11/11 00:00:00 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static t_rgb	get_phong_texture(const t_material *self,
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

static int	phong_scatter(const t_material *self,
	struct s_scatter_ctx *ctx)
{
	t_vec3	reflect_dir;
	t_rgb	base;
	t_rgb	texc;

	reflect_dir = vec3_reflect(ctx->r_in->direction, ctx->rec->normal);
	ctx->scattered->origin = apply_surface_bias(ctx->rec->p, reflect_dir,
			ctx->rec->normal);
	ctx->scattered->direction = vec3_normalize(reflect_dir);
	texc = get_phong_texture(self, ctx->rec);
	base = rgb_modulate(ctx->rec->rgb, texc);
	*ctx->attenuation = rgb_modulate(self->albedo, base);
	return (1);
}

t_material	*material_phong(t_rgb albedo, double shininess)
{
	t_material	*m;

	m = (t_material *)malloc(sizeof(t_material));
	if (!m)
		return (NULL);
	m->scatter = &phong_scatter;
	m->albedo = albedo;
	m->fuzz = 0.0;
	m->type = PHONG;
	m->refraction_index = 1.0;
	m->texture_type = NONE;
	m->texture_a = (t_rgb){255.0, 255.0, 255.0};
	m->texture_b = (t_rgb){0.0, 0.0, 0.0};
	m->texture_scale = 1.0;
	if (shininess < 1.0)
		m->shininess = 1.0;
	else
		m->shininess = shininess;
	m->specular = (t_rgb){255.0, 255.0, 255.0};
	return (m);
}
