#include "mini_rt.h"

static int	lambertian_scatter(const t_material *self, const t_ray *r_in,
		const t_hit_record *rec, t_rgb *attenuation, t_ray *scattered)
{
	t_vec3	dir;
	double	eps;
	double	sign;
	t_vec3	bias;

	(void)r_in;
	dir = random_on_hemisphere((t_vec3 *)&rec->normal);
	eps = 1e-4;
	sign = vec3_dot(dir, (t_vec3)rec->normal) > 0.0 ? 1.0 : -1.0;
	bias = vec3_multiply((t_vec3)rec->normal, eps * sign);
	scattered->origin = vec3_add(rec->p, bias);
	scattered->direction = dir;
	// scattered->direction = vec3_add_inline(&rec->normal, &scattered->direction);
	*attenuation = self->albedo;
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
	return (m);
}
