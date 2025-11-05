/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 10:52:20 by anakin            #+#    #+#             */
/*   Updated: 2025/10/31 11:45:44 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static t_rgb	calculate_final_color(t_rgb *final, t_ray *current_ray)
{
	t_vec3	unit_direction;
	double	a;
	t_vec3	temp1;
	t_vec3	temp2;
	t_vec3	color_a;
	t_vec3	color_b;
	t_vec3	result;
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
	temp1 = vec3_multiply(color_a, 1.0 - a);
	temp2 = vec3_multiply(color_b, a);
	result = vec3_add(temp1, temp2);
	sky_color.r = fmin(fmax(result.x, 0.0), 1.0) * 255.999;
	sky_color.g = fmin(fmax(result.y, 0.0), 1.0) * 255.999;
	sky_color.b = fmin(fmax(result.z, 0.0), 1.0) * 255.999;
	return (rgb_modulate(*final, sky_color));
}

static t_rgb	calculate_direct_lighting(t_data *data, const t_hit_record *rec)
{
	t_rgb			total_light;
	t_light			*light;
	int				hits;
	int				samples;
	double			light_radius;
	t_rgb			light_contrib;
	t_vec3			offset;
	t_vec3			sample_point;
	t_vec3			to_light;
	double			distance;
	t_vec3			light_dir;
	t_ray			shadow_ray;
	t_hit_record	shadow_rec;
	double			diffuse;

	total_light.r = (data->ambiente.color.r / 255.0) * (rec->rgb.r / 255.0)
		* data->ambiente.lighting * 255.0;
	total_light.g = (data->ambiente.color.g / 255.0) * (rec->rgb.g / 255.0)
		* data->ambiente.lighting * 255.0;
	total_light.b = (data->ambiente.color.b / 255.0) * (rec->rgb.b / 255.0)
		* data->ambiente.lighting * 255.0;
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
			if (!((USE_BVH && data->bvh_root && world_hit_bvh(data->bvh_root,
							data->objects, &shadow_ray, 0.001, distance - 0.001,
							&shadow_rec)) || (!USE_BVH && data->objects
						&& world_hit(data->objects, &shadow_ray, 0.001, distance
							- 0.001, &shadow_rec))))
			{
				diffuse = fmax(0.0, vec3_dot(rec->normal, light_dir));
				light_contrib.r += (light->color.r / 255.0) * (rec->rgb.r
						/ 255.0) * light->intensity * diffuse * 255.0;
				light_contrib.g += (light->color.g / 255.0) * (rec->rgb.g
						/ 255.0) * light->intensity * diffuse * 255.0;
				light_contrib.b += (light->color.b / 255.0) * (rec->rgb.b
						/ 255.0) * light->intensity * diffuse * 255.0;
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

/**
 * Samples an RGB color from the attached PNG at uv
 * Returns sRGB in 0..255 to match the renderer’s color space
 */
static t_rgb	sample_bump_rgb(const t_bump *b, double u, double v)
{
	uint32_t	ix;
	uint32_t	iy;
	size_t		idx;
	double		ur;
	double		vr;
	double		r;
	double		g;
	double		bl;

	if (!b || !b->pixels || !b->width || !b->height)
		return ((t_rgb){255.0, 255.0, 255.0});
	ur = u;
	vr = v;
	while (ur < 0.0)
		ur += 1.0;
	while (vr < 0.0)
		vr += 1.0;
	if (ur > 1.0)
		ur = ur - floor(ur);
	if (vr > 1.0)
		vr = vr - floor(vr);
	ix = (uint32_t)fmin((double)(b->width - 1), floor(ur * (double)b->width));
	iy = (uint32_t)fmin((double)(b->height - 1), floor(vr * (double)b->height));
	idx = ((size_t)iy * (size_t)b->width + (size_t)ix) * 4;
	r = (double)b->pixels[idx + 0];
	g = (double)b->pixels[idx + 1];
	bl = (double)b->pixels[idx + 2];
	return ((t_rgb){r, g, bl});
}

/**
 * Traces a ray with multiple bounces and accumulates color.
 * Applies bump mapping for normals and uses the PNG as albedo if present.
 */
t_rgb	ray_color(t_ray *initial_ray, t_data *data, int max_depth)
{
	t_ray			current_ray;
	t_rgb			final_color;
	t_rgb			throughput;
	t_hit_record	rec;
	t_rgb			direct_light;
	t_rgb			direct_contrib;
	int				depth;
	t_vec3			bumped;
	int				front;
				t_ray scattered;
				t_rgb attenuation;
					double max_throughput;
					double brightness;
	t_rgb			sky;

	current_ray = *initial_ray;
	final_color = (t_rgb){0.0, 0.0, 0.0};
	throughput = (t_rgb){255.0, 255.0, 255.0};
	depth = 0;
	while (depth < max_depth)
	{
		ft_bzero(&rec, sizeof(rec));
		if ((USE_BVH && world_hit_bvh(data->bvh_root, data->objects,
					&current_ray, 0.001, INFINITY, &rec)) || (!USE_BVH
				&& data->objects && world_hit(data->objects, &current_ray,
					0.001, INFINITY, &rec)))
		{
			if (rec.bump)
			{
				bumped = bump_perturb_from_uv(rec.bump, rec.normal, rec.tangent,
						rec.bitangent, rec.u, rec.v);
				front = (vec3_dot(current_ray.direction, bumped) < 0.0);
				rec.normal = front ? bumped : vec3_multiply(bumped, -1.0);
				rec.front_face = front;
				rec.rgb = sample_bump_rgb(rec.bump, rec.u, rec.v);
			}
			direct_light = calculate_direct_lighting(data, &rec);
			if (rec.mat)
			{
				if (rec.mat->scatter(rec.mat, &current_ray, &rec, &attenuation,
						&scattered))
				{
					direct_contrib = rgb_modulate(throughput, direct_light);
					final_color = rgb_add(final_color, direct_contrib);
					throughput = rgb_modulate(throughput, attenuation);
					if (depth >= 3)
					{
						max_throughput = fmax(fmax(throughput.r, throughput.g),
								throughput.b);
						brightness = max_throughput / 255.0;
						if (brightness < 0.1)
						{
							if (random_double() > brightness)
								return (final_color);
							throughput = rgb_multiply(throughput, 1.0
									/ brightness);
						}
					}
					current_ray = scattered;
					depth++;
					continue ;
				}
			}
			final_color = rgb_add(final_color, rgb_modulate(throughput,
						direct_light));
			return (final_color);
		}
		{
			sky = calculate_final_color(&final_color, &current_ray);
			final_color = rgb_add(final_color, rgb_modulate(throughput, sky));
			return (final_color);
		}
	}
	return (final_color);
}
