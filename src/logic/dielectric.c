#include "mini_rt.h"

static double	schlick_reflectance(double cosine, double ref_idx)
{
	double	r0;

	r0 = (1.0 - ref_idx) / (1.0 + ref_idx);
	r0 = r0 * r0;
	return (r0 + (1.0 - r0) * pow(1.0 - cosine, 5.0));
}

static int	dielectric_scatter(const t_material *self,
	struct s_scatter_ctx *ctx)
{
	double	ri;
	t_vec3	dir;
	t_vec3	neg_dir;
	double	cos_theta;
	double	sin_theta_sq;
	double	sin_theta;
	int		cannot_refract;
	double	choose_reflect;
	t_rgb	texc;
	double	s;
	int		m;
	t_rgb	base;

	ri = ctx->rec->front_face ? (1.0 / self->refraction_index) : self->refraction_index;
	dir = vec3_normalize(ctx->r_in->direction);
	neg_dir = vec3_overload(dir);
	cos_theta = fmin(vec3_dot(neg_dir, (t_vec3)ctx->rec->normal), 1.0);
	sin_theta_sq = 1.0 - cos_theta * cos_theta;
	sin_theta = sin_theta_sq > 0.0 ? sqrt(sin_theta_sq) : 0.0;
	cannot_refract = (ri * sin_theta) > 1.0;
	choose_reflect = schlick_reflectance(cos_theta, ri);
	if (cannot_refract || choose_reflect > random_double())
		dir = vec3_reflect(dir, ctx->rec->normal);
	else
		dir = vec3_refract(dir, ctx->rec->normal, ri);
	ctx->scattered->origin = apply_surface_bias(ctx->rec->p, dir,
			ctx->rec->normal);
	ctx->scattered->direction = dir;
	if (self->texture_type == CHECKER)
	{
		s = (self->texture_scale <= 0.0) ? 1.0 : self->texture_scale;
		m = (((int)floor(ctx->rec->u * s)) + ((int)floor(ctx->rec->v * s))) & 1;
		texc = m ? self->texture_b : self->texture_a;
	}
	else
		texc = (t_rgb){255.0, 255.0, 255.0};
	base = rgb_modulate((t_rgb){255.0, 255.0, 255.0}, ctx->rec->rgb);
	*ctx->attenuation = rgb_modulate(base, texc);
	return (1);
}

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
