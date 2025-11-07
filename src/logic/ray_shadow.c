/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_shadow.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 13:21:00 by anakin            #+#    #+#             */
/*   Updated: 2025/11/07 13:21:00 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static void	calc_shadow_sample(t_data *data, t_light *light,
		const t_hit_record *rec, t_shadow_calc *sc)
{
	sc->offset = random_unit_vec3();
	sc->offset = vec3_multiply(sc->offset, 0.5);
	sc->sample_point = vec3_add(light->cords, sc->offset);
	sc->to_light = vec3_sub(sc->sample_point, rec->p);
	sc->distance = sqrt(vec3_dot(sc->to_light, sc->to_light));
	sc->light_dir = vec3_divide(sc->to_light, sc->distance);
	sc->shadow_ray.origin = rec->p;
	sc->shadow_ray.direction = sc->light_dir;
	if (!is_in_shadow(data, sc))
	{
		sc->diffuse = fmax(0.0, vec3_dot(rec->normal, sc->light_dir));
	}
}

static void	process_light_samples(t_data *data, t_light *light,
		const t_hit_record *rec, t_rgb *light_contrib)
{
	int				hits;
	int				samples;
	t_shadow_calc	sc;

	hits = 0;
	samples = 0;
	while (samples < SHADOW_SAMPLES)
	{
		calc_shadow_sample(data, light, rec, &sc);
		if (!is_in_shadow(data, &sc))
		{
			sc.diffuse = fmax(0.0, vec3_dot(rec->normal, sc.light_dir));
			add_light_sample(light_contrib, light, rec, sc.diffuse);
			hits++;
		}
		samples++;
	}
	if (hits > 0)
	{
		light_contrib->r /= samples;
		light_contrib->g /= samples;
		light_contrib->b /= samples;
	}
}

t_rgb	calculate_direct_lighting(t_data *data, const t_hit_record *rec)
{
	t_rgb	total_light;
	t_light	*light;
	t_rgb	light_contrib;

	total_light = get_ambient_light(data, rec);
	if (!data->settings.light_state || !data->light_list)
		return (total_light);
	light = data->light_list->head;
	while (light)
	{
		light_contrib = (t_rgb){0.0, 0.0, 0.0};
		process_light_samples(data, light, rec, &light_contrib);
		if (light_contrib.r > 0.0 || light_contrib.g > 0.0
			|| light_contrib.b > 0.0)
			total_light = rgb_add(total_light, light_contrib);
		light = light->next;
	}
	return (total_light);
}
