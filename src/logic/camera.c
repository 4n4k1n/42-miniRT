/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:01:30 by apregitz          #+#    #+#             */
/*   Updated: 2025/10/27 10:32:55 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

void	get_camera_vectors(t_data *data, t_vec3 *forward, t_vec3 *right, t_vec3 *up)
{
	forward->x = cos(data->camera.pitch) * cos(data->camera.yaw);
	forward->y = sin(data->camera.pitch);
	forward->z = cos(data->camera.pitch) * sin(data->camera.yaw);
	*up = vec3_init(0.0, 1.0, 0.0);
	*right = vec3_cross_ptr(forward, up);
	*right = vec3_divide(*right, vec3_sqrt_ptr(right));
	*up = vec3_cross_ptr(right, forward);
	*up = vec3_divide(*up, vec3_sqrt_ptr(up));
}

void	update_camera(t_data *data)
{
	t_init_tmp tmp;
	t_vec3 forward;
	t_vec3 right;
	t_vec3 up;

	get_camera_vectors(data, &forward, &right, &up);
	
	data->camera.viewport_u = vec3_multiply(right, data->camera.viewport_width);
	data->camera.viewport_v = vec3_multiply(up, -data->camera.viewport_height);
	data->camera.pixel_delta_u = vec3_divide(data->camera.viewport_u, WIDTH);
	data->camera.pixel_delta_v = vec3_divide(data->camera.viewport_v, HEIGHT);
	
	tmp.t1 = vec3_multiply(forward, data->camera.foc);
	tmp.t2 = vec3_sub(data->camera.cords, tmp.t1);
	tmp.t3 = vec3_divide(data->camera.viewport_u, 2.0);
	tmp.t4 = vec3_sub(tmp.t2, tmp.t3);
	tmp.t5 = vec3_divide(data->camera.viewport_v, 2.0);
	data->camera.viewport_upper_left = vec3_sub(tmp.t4, tmp.t5);
	tmp.t6 = vec3_add(data->camera.pixel_delta_u, data->camera.pixel_delta_v);
	tmp.t7 = vec3_multiply(tmp.t6, 0.5);
	data->camera.pixel00_loc = vec3_add(data->camera.viewport_upper_left, tmp.t7);
}

static inline t_rgb	rgb_multiply_inline(t_rgb color, double t)
{
	color.r *= t;
	color.g *= t;
	color.b *= t;
	return (color);
}

static inline t_rgb	rgb_modulate_inline(t_rgb a, t_rgb b)
{
	t_rgb out;
	out.r = (a.r * b.r) / 255.999;
	out.g = (a.g * b.g) / 255.999;
	out.b = (a.b * b.b) / 255.999;
	return out;
}

static inline t_rgb	rgb_add_inline(t_rgb a, t_rgb b)
{
	t_rgb out;
	out.r = fmin(a.r + b.r, 255.999);
	out.g = fmin(a.g + b.g, 255.999);
	out.b = fmin(a.b + b.b, 255.999);
	return out;
}

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
			total_light = rgb_add_inline(total_light, light_contrib);
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
	return (rgb_modulate_inline(*final, sky_color));
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
					direct_contrib = rgb_modulate_inline(throughput, direct_light);
					direct_contrib = rgb_modulate_inline(direct_contrib, attenuation);
					final_color = rgb_add_inline(final_color, direct_contrib);
					throughput = rgb_modulate_inline(throughput, attenuation);
					current_ray = scattered;
				}
				else
					return (final_color);
			}
			else
			{
				direct_contrib = rgb_modulate_inline(direct_light, rec.rgb);
				final_color = rgb_add_inline(final_color, direct_contrib);
				return (final_color);
			}
		}
		else
		{
			t_rgb sky = calculate_final_color(&throughput, &current_ray);
			return (rgb_add_inline(final_color, sky));
		}
		depth++;
	}
	return (final_color);
}

/**
 * Calculates image height based on width and aspect ratio
 * Formula: height = width / aspect_ratio
 * Ensures minimum height of 1 pixel
 */
static int	get_image_height(int width, double ra)
{
	int	height;

	height = width / ra;
	if (height < 1)
		return (1);
	return (height);
}

/**
 * Initializes camera parameters for ray tracing
 * Sets up viewport dimensions, pixel deltas, and starting pixel location
 * Creates coordinate system for converting screen pixels to world rays
 */
void	init_camera(t_data *data)
{
	data->width = WIDTH;
	data->height = get_image_height(WIDTH, ASPECT_RATIO);
	data->camera.foc = 1.0;
	data->camera.viewport_height = 2.0;
	data->camera.viewport_width = data->camera.viewport_height * ASPECT_RATIO;
	data->camera.cords = vec3_init(0.0, 0.0, 0.0);
	data->camera.pitch = 0.0;
	data->camera.yaw = 0.0;
	update_camera(data);
}
