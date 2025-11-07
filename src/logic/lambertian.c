#include "mini_rt.h"

static int	lambertian_scatter(const t_material *self, const t_ray *r_in,
		const t_hit_record *rec, t_rgb *attenuation, t_ray *scattered)
{
	t_vec3	dir;
	t_rgb	base;
	t_rgb	texc;
	double	s;
	int		m;

	(void)r_in;
	dir = random_on_hemisphere((t_vec3 *)&rec->normal);
	scattered->origin = apply_surface_bias(rec->p, dir, rec->normal);
	scattered->direction = dir;
	if (self->texture_type == CHECKER)
	{
		s = (self->texture_scale <= 0.0) ? 1.0 : self->texture_scale;
		m = (((int)floor(rec->u * s)) + ((int)floor(rec->v * s))) & 1;
		texc = m ? self->texture_b : self->texture_a;
	}
	else
		texc = (t_rgb){255.0, 255.0, 255.0};
	base = rgb_modulate(rec->rgb, texc);
	*attenuation = rgb_modulate(self->albedo, base);
	return (1);
}

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
