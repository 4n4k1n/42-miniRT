/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_lighting.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 13:16:00 by anakin            #+#    #+#             */
/*   Updated: 2025/11/07 13:20:00 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

t_rgb	get_ambient_light(t_data *data, const t_hit_record *rec)
{
	t_rgb	ambient;

	ambient.r = (data->ambiente.color.r / 255.0) * (rec->rgb.r / 255.0)
		* data->ambiente.lighting * 255.0;
	ambient.g = (data->ambiente.color.g / 255.0) * (rec->rgb.g / 255.0)
		* data->ambiente.lighting * 255.0;
	ambient.b = (data->ambiente.color.b / 255.0) * (rec->rgb.b / 255.0)
		* data->ambiente.lighting * 255.0;
	return (ambient);
}

int	is_in_shadow(t_data *data, t_shadow_calc *sc)
{
	if (data->settings.use_bvh && data->bvh_root
		&& world_hit_bvh(data->bvh_root, data->objects,
			&sc->shadow_ray, 0.001, sc->distance - 0.001, &sc->shadow_rec))
		return (1);
	if (!data->settings.use_bvh && data->objects
		&& world_hit(data->objects, &sc->shadow_ray, 0.001, sc->distance
			- 0.001, &sc->shadow_rec))
		return (1);
	return (0);
}

void	add_light_sample(t_rgb *light_contrib, t_light *light,
		const t_hit_record *rec, double diffuse)
{
	light_contrib->r += (light->color.r / 255.0) * (rec->rgb.r
			/ 255.0) * light->intensity * diffuse * 255.0;
	light_contrib->g += (light->color.g / 255.0) * (rec->rgb.g
			/ 255.0) * light->intensity * diffuse * 255.0;
	light_contrib->b += (light->color.b / 255.0) * (rec->rgb.b
			/ 255.0) * light->intensity * diffuse * 255.0;
}

t_rgb	calculate_final_color(t_rgb *final, t_ray *current_ray)
{
	t_vec3	unit_direction;
	double	a;
	t_vec3	color_a;
	t_vec3	color_b;
	t_rgb	sky_color;

	color_a = (t_vec3){1.0, 1.0, 1.0};
	color_b = (t_vec3){0.5, 0.7, 1.0};
	if (!SKY)
	{
		color_a = vec3_init(0.0, 0.0, 0.0);
		color_b = vec3_init(0.0, 0.0, 0.0);
	}
	unit_direction = vec3_normalize(current_ray->direction);
	a = 0.5 * (unit_direction.y + 1.0);
	color_a = vec3_multiply(color_a, 1.0 - a);
	color_b = vec3_multiply(color_b, a);
	unit_direction = vec3_add(color_a, color_b);
	sky_color.r = fmin(fmax(unit_direction.x, 0.0), 1.0) * 255.999;
	sky_color.g = fmin(fmax(unit_direction.y, 0.0), 1.0) * 255.999;
	sky_color.b = fmin(fmax(unit_direction.z, 0.0), 1.0) * 255.999;
	return (rgb_modulate(*final, sky_color));
}
