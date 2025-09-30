/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 11:00:01 by apregitz          #+#    #+#             */
/*   Updated: 2025/09/30 19:25:15 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static void	build_demo_world(t_data *data)
{
	t_obj	*s1;
	t_obj	*s2;

	data->objects = (t_obj_list *)malloc(sizeof(t_obj_list));
	if (!data->objects)
		return;
	data->objects->head = NULL;
	data->objects->tail = NULL;
	data->objects->size = 0;

	// Sphere at (0,0,-1), radius 0.5 -> diameter 1.0
	s1 = obj_new(SPHERE);
	if (s1)
	{
		s1->data.sphere.cords = vec3_init_inline(0.0, 0.0, -1.5);
		s1->data.sphere.diameter = 1.0;
		s1->data.sphere.rgb = (t_rgb){255, 255, 255};
		obj_push(data->objects, s1);
	}

	// Ground sphere at (0,-100.5,-1), radius 100 -> diameter 200
	s2 = obj_new(SPHERE);
	if (s2)
	{
		s2->data.sphere.cords = vec3_init_inline(0.0, -100.5, -1.0);
		s2->data.sphere.diameter = 200.0;
		s2->data.sphere.rgb = (t_rgb){255, 255, 255};
		obj_push(data->objects, s2);
	}
}

int	main(int ac, char **av)
{
	t_data	data;
	int		i;
	int		j;

	(void)ac;
	(void)av;
	data.camera.samples_per_pixel = 128; // AA
	init_camera(&data);
	build_demo_world(&data);
	mlx_set_setting(MLX_MAXIMIZED, false);
	mlx_t* mlx = mlx_init(WIDTH, HEIGHT, "miniRT", false);
	if (!mlx)
		return (1);
	mlx_image_t* img = mlx_new_image(mlx, WIDTH, HEIGHT);
	if (!img || (mlx_image_to_window(mlx, img, 0, 0) < 0))
		return (1);
	i = 0;
	while (i < HEIGHT)
	{
		j = 0;
		while (j < WIDTH)
		{
			const int   max_spp = (data.camera.samples_per_pixel > 0) ? data.camera.samples_per_pixel : 1;
			const int   min_spp = (AA_MIN_SAMPLES < max_spp) ? AA_MIN_SAMPLES : max_spp;
			double      acc_r = 0.0, acc_g = 0.0, acc_b = 0.0;
			int         n = 0;
			double      mean = 0.0;
			double      m2 = 0.0;
			while (n < max_spp)
			{
				double off_u = random_double() - 0.5;
				double off_v = random_double() - 0.5;
				t_vec3 temp_u = vec3_multiply_inline(&data.camera.pixel_delta_u, (double)j + off_u);
				t_vec3 temp_v = vec3_multiply_inline(&data.camera.pixel_delta_v, (double)i + off_v);
				t_vec3 temp_offset = vec3_add_inline(&temp_u, &temp_v);
				t_vec3 pixel_sample = vec3_add_inline(&data.camera.pixel00_loc, &temp_offset);
				t_vec3 ray_direction = vec3_sub_inline(&pixel_sample, &data.camera.cords);
				t_ray ray;
				ray.origin = data.camera.cords;
				ray.direction = ray_direction;
				t_rgb sample = ray_color(&ray, data.objects);
				acc_r += sample.r;
				acc_g += sample.g;
				acc_b += sample.b;
				double luma = (0.2126 * sample.r + 0.7152 * sample.g + 0.0722 * sample.b) / 255.999;
				++n;
				double delta = luma - mean;
				mean += delta / (double)n;
				double delta2 = luma - mean;
				m2 += delta * delta2;
				if (n >= min_spp)
				{
					double var = (n > 1) ? (m2 / (double)(n - 1)) : 0.0;
					if (var <= AA_VAR_EPS)
						break;
				}
			}
			const double inv_n = 1.0 / (double)n;
			double avg_r = acc_r * inv_n;
			double avg_g = acc_g * inv_n;
			double avg_b = acc_b * inv_n;
			int rbyte = (int)(256.0 * clamp(avg_r / 255.999, 0.0, 0.999));
			int gbyte = (int)(256.0 * clamp(avg_g / 255.999, 0.0, 0.999));
			int bbyte = (int)(256.0 * clamp(avg_b / 255.999, 0.0, 0.999));

			t_rgb color = { (double)rbyte, (double)gbyte, (double)bbyte };
			uint32_t packed = rgb_to_uint32(&color);
			mlx_put_pixel(img, j, i, packed);
			j++;
		}
		i++;
	}
	mlx_loop(mlx);
	mlx_terminate(mlx);
	return (0);
}
