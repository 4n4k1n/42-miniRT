#include "mini_rt.h"

static double	schlick_reflectance(double cosine, double refraction_index)
{
	double	r0;

	r0 = (1.0 - refraction_index) / (1.0 + refraction_index);
	r0 = r0 * r0;
	return (r0 + (1.0 - r0) * pow(1.0 - cosine, 5.0));
}

static t_vec3	vec3_refract(const t_vec3 uv, const t_vec3 n, \
		double etai_over_etat)
{
	t_vec3	neg_uv;
	double	cos_theta;
	t_vec3	term1;
	t_vec3	uv_plus;
	t_vec3	r_out_perp;
	double	k;
	double	parallel_scale;
	t_vec3	r_out_parallel;

	neg_uv = vec3_multiply(uv, -1.0);
	cos_theta = fmin(vec3_dot(neg_uv, n), 1.0);
	term1 = vec3_multiply(n, cos_theta);
	uv_plus = vec3_add(uv, term1);
	r_out_perp = vec3_multiply(uv_plus, etai_over_etat);
	k = 1.0 - vec3_dot(r_out_perp, r_out_perp);
	if (k < 0.0)
		k = 0.0;
	parallel_scale = -sqrt(k);
	r_out_parallel = vec3_multiply(n, parallel_scale);
	return (vec3_add(r_out_perp, r_out_parallel));
}

static int	dielectric_scatter(const t_material *self, const t_ray *r_in,
	const t_hit_record *rec, t_rgb *attenuation, t_ray *scattered)
{
	t_vec3	dir;
	double	ri;
	t_vec3	neg_dir;
	double	cos_theta;
	double	sin_theta_sq;
	double	sin_theta;
	int		cannot_refract;
	double	choose_reflect;
	t_vec3	direction;

	dir = vec3_normalize(r_in->direction);
	ri = rec->front_face ? (1.0 / self->refraction_index) : self->refraction_index;
	neg_dir = vec3_multiply(dir, -1.0);
	cos_theta = fmin(vec3_dot(neg_dir, (t_vec3)rec->normal), 1.0);
	sin_theta_sq = 1.0 - cos_theta * cos_theta;
	sin_theta = sin_theta_sq > 0.0 ? sqrt(sin_theta_sq) : 0.0;
	cannot_refract = (ri * sin_theta) > 1.0;
	choose_reflect = schlick_reflectance(cos_theta, ri);
	if (cannot_refract || choose_reflect > random_double())
		direction = vec3_reflect(dir, rec->normal);
	else
		direction = vec3_refract(dir, rec->normal, ri);
	scattered->origin = apply_surface_bias(rec->p, direction, rec->normal);
	scattered->direction = direction;
	attenuation->r = 255.0;
	attenuation->g = 255.0;
	attenuation->b = 255.0;
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
	return (m);
}