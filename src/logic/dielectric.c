/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dielectric.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 00:00:00 by nweber            #+#    #+#             */
/*   Updated: 2025/11/12 13:35:42 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Schlick's approximation for Fresnel reflectance.
 * @param cosine angle between incident ray and surface normal (cos theta)
 * @param ref_idx relative index of refraction
 * @return approximate reflectance in [0,1]
 */
static double	schlick_reflectance(double cosine, double ref_idx)
{
	double	r0;

	r0 = (1.0 - ref_idx) / (1.0 + ref_idx);
	r0 = r0 * r0;
	return (r0 + (1.0 - r0) * pow(1.0 - cosine, 5.0));
}

/**
 * Scatter function for dielectric (transparent) materials.
 * Chooses reflection or refraction using total internal reflection
 * and Schlick's reflectance probability. Sets scattered ray origin/direction
 * and attenuation modulated by the underlying texture/color.
 * @param self material descriptor (contains refraction index, textures)
 * @param ctx  scatter context (reads r_in, rec; writes scattered, attenuation)
 * @return 1 always (ray continues as either reflection or refraction)
 */
static int	dielectric_scatter(const t_material *self,
	struct s_scatter_ctx *ctx)
{
	double	ri;
	t_vec3	dir;
	double	cos_theta;
	double	sin_theta;
	t_rgb	base;

	ri = get_refraction_index(self, ctx->rec->front_face);
	dir = vec3_normalize(ctx->r_in->direction);
	cos_theta = fmin(vec3_dot(vec3_overload(dir),
				(t_vec3)ctx->rec->normal), 1.0);
	sin_theta = get_sin_theta(cos_theta);
	if ((ri * sin_theta) > 1.0
		|| schlick_reflectance(cos_theta, ri) > random_double())
		dir = vec3_reflect(dir, ctx->rec->normal);
	else
		dir = vec3_refract(dir, ctx->rec->normal, ri);
	ctx->scattered->origin = apply_surface_bias(ctx->rec->p, dir,
			ctx->rec->normal);
	ctx->scattered->direction = dir;
	base = rgb_modulate((t_rgb){255.0, 255.0, 255.0}, ctx->rec->rgb);
	*ctx->attenuation = rgb_modulate(base,
			get_texture_color(self, ctx->rec->u, ctx->rec->v));
	return (1);
}

/**
 * Allocates and initializes a dielectric material.
 * @param refraction_index index of refraction for the material
 * @return pointer to allocated t_material or NULL on OOM
 */
t_material	*material_dielectric(double refraction_index)
{
	t_material	*m;

	m = (t_material *)malloc(sizeof(t_material));
	if (!m)
		return (NULL);
	m->scatter = &dielectric_scatter;
	m->albedo = (t_rgb){255.0, 255.0, 255.0};
	m->fuzz = 0.0;
	m->type = 2;
	m->refraction_index = refraction_index;
	m->texture_type = NONE;
	m->texture_a = (t_rgb){255.0, 255.0, 255.0};
	m->texture_b = (t_rgb){0.0, 0.0, 0.0};
	m->texture_scale = 1.0;
	return (m);
}
