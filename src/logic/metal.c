/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   metal.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 14:33:25 by anakin            #+#    #+#             */
/*   Updated: 2025/10/30 14:34:57 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static int	metal_scatter(const t_material *self, const t_ray *r_in,
		const t_hit_record *rec, t_rgb *attenuation, t_ray *scattered)
{
	t_vec3	in_dir;
	t_vec3	reflected;
	t_vec3	rnd;
	t_vec3	fuzz_off;

	in_dir = vec3_normalize(r_in->direction);
	reflected = vec3_normalize(vec3_reflect(in_dir, rec->normal));
	if (self->fuzz > 0.0)
	{
		rnd = random_on_hemisphere((t_vec3 *)&rec->normal);
		fuzz_off = vec3_multiply(rnd, self->fuzz);
		reflected = vec3_add(reflected, fuzz_off);
	}
	scattered->origin = apply_surface_bias(rec->p, reflected, rec->normal);
	scattered->direction = reflected;
	*attenuation = self->albedo;
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
	return (m);
}
