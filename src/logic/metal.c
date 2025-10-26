#include "mini_rt.h"

static int	metal_scatter(const t_material *self, const t_ray *r_in,
		const t_hit_record *rec, t_rgb *attenuation, t_ray *scattered)
{
	t_vec3	in_dir;
	double	len;
	t_vec3	reflected;
	double	rlen;
	t_vec3	rnd;
	t_vec3	fuzz_off;
	double	eps;
	double	sign;
	t_vec3	bias;

	in_dir = r_in->direction;
	len = vec3_sqrt(in_dir);
	if (len != 0.0)
		in_dir = vec3_divide(in_dir, len);
	reflected = vec3_reflect_inline(&in_dir, &rec->normal);
	rlen = vec3_sqrt(reflected);
	if (rlen != 0.0)
		reflected = vec3_divide(reflected, rlen);
	if (self->fuzz > 0.0)
	{
		rnd = random_on_hemisphere((t_vec3 *)&rec->normal);
		fuzz_off = vec3_multiply(rnd, self->fuzz);
		reflected = vec3_add(reflected, fuzz_off);
	}
	eps = 1e-4;
	sign = vec3_dot(reflected, rec->normal) > 0.0 ? 1.0 : -1.0;
	bias = vec3_multiply((t_vec3)rec->normal, eps * sign);
	scattered->origin = vec3_add(rec->p, bias);
	scattered->direction = reflected;
	*attenuation = self->albedo;
	return (vec3_dot(scattered->direction, rec->normal) > 0.0);
}

t_material	*material_metal(t_rgb albedo, double fuzz)
{
	t_material *m = (t_material *)malloc(sizeof(t_material));
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
	return (m);
}
