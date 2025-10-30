/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lambertian.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 14:33:16 by anakin            #+#    #+#             */
/*   Updated: 2025/10/30 14:34:47 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static int	lambertian_scatter(const t_material *self, const t_ray *r_in,
		const t_hit_record *rec, t_rgb *attenuation, t_ray *scattered)
{
	t_vec3	dir;

	(void)r_in;
	dir = random_on_hemisphere((t_vec3 *)&rec->normal);
	scattered->origin = apply_surface_bias(rec->p, dir, rec->normal);
	scattered->direction = dir;
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
