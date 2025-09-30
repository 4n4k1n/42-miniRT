/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 11:00:01 by apregitz          #+#    #+#             */
/*   Updated: 2025/09/30 17:07:00 by nweber           ###   ########.fr       */
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
		s1->data.sphere.cords = vec3_init_inline(0.0, 0.0, -1.0);
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

	// Initialize camera
	init_camera(&data);

	// Build a simple world of hittables (two spheres)
	build_demo_world(&data);

	mlx_set_setting(MLX_MAXIMIZED, false);
	mlx_t* mlx = mlx_init(WIDTH, HEIGHT, "miniRT", false);
	if (!mlx)
		return (1);
	mlx_image_t* img = mlx_new_image(mlx, WIDTH, HEIGHT);
	if (!img || (mlx_image_to_window(mlx, img, 0, 0) < 0))
		return (1);

	// Render
	i = 0;
	while (i < HEIGHT)
	{
		j = 0;
		while (j < WIDTH)
		{
			// Calculate ray for this pixel
			t_vec3 temp_u = vec3_multiply_inline(&data.camera.pixel_delta_u, j);
			t_vec3 temp_v = vec3_multiply_inline(&data.camera.pixel_delta_v, i);
			t_vec3 temp_offset = vec3_add_inline(&temp_u, &temp_v);
			t_vec3 pixel_center = vec3_add_inline(&data.camera.pixel00_loc, &temp_offset);
			t_vec3 ray_direction = vec3_sub_inline(&pixel_center, &data.camera.cords);

			t_ray ray;
			ray.origin = data.camera.cords;
			ray.direction = ray_direction;

			// Get color from world
			t_rgb color = ray_color(&ray, data.objects);
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
