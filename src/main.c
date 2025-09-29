/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 11:00:01 by apregitz          #+#    #+#             */
/*   Updated: 2025/09/29 21:35:36 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

int	main(int ac, char **av)
{
	t_data	data;
	int	i;
	int	j;
	
	(void)ac;
	(void)av;

	// Initialize camera
	init_camera(&data);
	
	mlx_set_setting(MLX_MAXIMIZED, false);
	mlx_t* mlx = mlx_init(WIDTH, HEIGHT, "miniRT", false);
	if (!mlx)
		return (1);
	mlx_image_t* img = mlx_new_image(mlx, WIDTH, HEIGHT);
	if (!img || (mlx_image_to_window(mlx, img, 0, 0) < 0))
		return (1);

	// Render the gradient
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
			t_vec3 ray_direction = vec3_sub_inline(&pixel_center, &data.camera.center);
			
			t_ray ray;
			ray.origin = data.camera.center;
			ray.direction = ray_direction;
			
			// Get color from ray
			t_rgb color = ray_color(&ray);
			
			// Convert to 0-255 range
			color.r = (int)(color.r * 255.0);
			color.g = (int)(color.g * 255.0);
			color.b = (int)(color.b * 255.0);
			
			mlx_put_pixel(img, j, i, rgb_to_uint32(&color));
			j++;
		}
		i++;
	}

	mlx_loop(mlx);
	mlx_terminate(mlx);

	return (0);
}
