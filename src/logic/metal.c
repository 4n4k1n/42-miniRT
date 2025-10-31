#include "mini_rt.h"

static int	metal_scatter(const t_material *self, const t_ray *r_in,
		const t_hit_record *rec, t_rgb *attenuation, t_ray *scattered)
{
	t_vec3	reflected;
	t_vec3	dir;
	t_rgb	base;
	t_rgb	texc;
	double	s;
	int		m;

	reflected = vec3_reflect(vec3_normalize(r_in->direction), rec->normal);
	dir = vec3_add(reflected, vec3_multiply(random_unit_vec3(), self->fuzz));
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
	base = rgb_modulate(self->albedo, rec->rgb);
	*attenuation = rgb_modulate(base, texc);
	return (vec3_dot(scattered->direction, rec->normal) > 0.0);
}

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
