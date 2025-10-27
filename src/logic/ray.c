/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 10:52:20 by anakin            #+#    #+#             */
/*   Updated: 2025/10/27 11:38:39 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static t_rgb	calculate_direct_lighting(t_data *data, t_hit_record *rec)
{
	t_light		*light;
	t_vec3		light_dir;
	t_vec3		to_light;
	t_vec3		sample_point;
	t_vec3		offset;
	double		distance;
	double		diffuse;
	t_rgb		light_contrib;
	t_rgb		total_light;
	t_ray		shadow_ray;
	t_hit_record	shadow_rec;
	int			samples;
	int			hits;
	double		light_radius;

	total_light.r = (data->ambiente.color.r / 255.0) * data->ambiente.lighting * 255.0;
	total_light.g = (data->ambiente.color.g / 255.0) * data->ambiente.lighting * 255.0;
	total_light.b = (data->ambiente.color.b / 255.0) * data->ambiente.lighting * 255.0;
	if (!data->settings.light_state || !data->light_list)
		return (total_light);
	light = data->light_list->head;
	while (light)
	{
		hits = 0;
		samples = 0;
		light_radius = 0.5;
		light_contrib = (t_rgb){0.0, 0.0, 0.0};
		while (samples < SHADOW_SAMPLES)
		{
			offset = random_unit_vec3();
			offset = vec3_multiply(offset, light_radius);
			sample_point = vec3_add(light->cords, offset);
			to_light = vec3_sub(sample_point, rec->p);
			distance = sqrt(vec3_dot(to_light, to_light));
			light_dir = vec3_divide(to_light, distance);
			shadow_ray.origin = rec->p;
			shadow_ray.direction = light_dir;
			if (!world_hit(data->objects, &shadow_ray, 0.001, distance - 0.001, &shadow_rec))
			{
				diffuse = fmax(0.0, vec3_dot(rec->normal, light_dir));
				light_contrib.r += (light->color.r / 255.0) * light->intensity * diffuse * 255.0;
				light_contrib.g += (light->color.g / 255.0) * light->intensity * diffuse * 255.0;
				light_contrib.b += (light->color.b / 255.0) * light->intensity * diffuse * 255.0;
				hits++;
			}
			samples++;
		}
		if (hits > 0)
		{
			light_contrib.r /= samples;
			light_contrib.g /= samples;
			light_contrib.b /= samples;
			total_light = rgb_add(total_light, light_contrib);
		}
		light = light->next;
	}
	return (total_light);
}

static t_rgb	calculate_final_color(t_rgb *final, t_ray *current_ray)
{
	double	len;
	t_vec3	unit_direction;
	double	a;
	t_vec3	temp1;
	t_vec3	temp2;
	t_vec3			color_a = (t_vec3){0.0, 0.0, 0.0};
	t_vec3			color_b = (t_vec3){0.0, 0.0, 0.0};
	t_vec3			result;
	t_rgb			sky_color;

	len = sqrt(vec3_dot(current_ray->direction, current_ray->direction));
	if (len != 0.0)
		unit_direction = vec3_divide(current_ray->direction, len);
	else
		unit_direction = vec3_cpy(current_ray->direction);
	a = 0.5 * (unit_direction.y + 1.0);
	temp1 = vec3_multiply(color_a, 1.0 - a);
	temp2 = vec3_multiply(color_b, a);
	result = vec3_add(temp1, temp2);
	sky_color.r = fmin(fmax(result.x, 0.0), 1.0) * 255.999;
	sky_color.g = fmin(fmax(result.y, 0.0), 1.0) * 255.999;
	sky_color.b = fmin(fmax(result.z, 0.0), 1.0) * 255.999;
	return (rgb_modulate(*final, sky_color));
}

/**
 * Determines the color for a ray by checking sphere intersection
 * If ray hits sphere: calculates surface normal and converts to color
 * If no hit: creates gradient background from white to blue
 * Formula for normal: N = (hit_point - sphere_center) / radius
 */
t_rgb	ray_color(t_ray *initial_ray, t_data *data, int max_depth)
{
	t_ray			current_ray = *initial_ray;
	t_rgb			final_color = (t_rgb){0.0, 0.0, 0.0};
	t_rgb			throughput = data->ambiente.color;
	t_hit_record	rec;
	t_rgb			direct_light;
	t_rgb			direct_contrib;
	int				depth;

	depth = 0;
	while (depth < max_depth)
	{
		if (data->objects && world_hit(data->objects, &current_ray, 0.001, INFINITY, &rec))
		{
			direct_light = calculate_direct_lighting(data, &rec);
			if (rec.mat)
			{
				t_ray scattered;
				t_rgb attenuation;
				if (rec.mat->scatter(rec.mat, &current_ray, &rec, &attenuation, &scattered))
				{
					direct_contrib = rgb_modulate(throughput, direct_light);
					direct_contrib = rgb_modulate(direct_contrib, attenuation);
					final_color = rgb_add(final_color, direct_contrib);
					throughput = rgb_modulate(throughput, attenuation);
					current_ray = scattered;
				}
				else
					return (final_color);
			}
			else
			{
				direct_contrib = rgb_modulate(direct_light, rec.rgb);
				final_color = rgb_add(final_color, direct_contrib);
				return (final_color);
			}
		}
		else
		{
			t_rgb sky = calculate_final_color(&throughput, &current_ray);
			return (rgb_add(final_color, sky));
		}
		depth++;
	}
	return (final_color);
}
