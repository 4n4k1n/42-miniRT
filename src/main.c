/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apregitz <apregitz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 11:00:01 by apregitz          #+#    #+#             */
/*   Updated: 2025/10/10 17:32:33 by apregitz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Creates a demo scene with two spheres for testing
 * Sets up object list and adds one small sphere and one large ground sphere
 * Used to test ray tracing functionality
 */
// static void	build_demo_world(t_data *data)
// {
//     t_obj	*s1;
//     t_obj	*s2;
//     // NEW demo spheres
//     t_obj	*s3;
//     t_obj	*s4;

//     data->objects = (t_obj_list *)malloc(sizeof(t_obj_list));
//     if (!data->objects)
//         return;
//     data->objects->head = NULL;
//     data->objects->tail = NULL;
//     data->objects->size = 0;

//     // Ground sphere: lambertian (0.8,0.8,0.0)
//     s2 = obj_new(SPHERE);
//     if (s2)
//     {
//         s2->data.sphere.cords = vec3_init_inline(0.0, -100.5, -1.0);
//         s2->data.sphere.diameter = 200.0;
//         s2->data.sphere.rgb = (t_rgb){255, 255, 255};
//         s2->data.sphere.mat = material_metal((t_rgb){204.0, 204.0, 204.0}, 0.0);
//         obj_push(data->objects, s2);
//     }

//     // Center sphere: lambertian (0.1,0.2,0.5)
//     s1 = obj_new(SPHERE);
//     if (s1)
//     {
//         s1->data.sphere.cords = vec3_init_inline(0.0, 0.0, -1.2);
//         s1->data.sphere.diameter = 1.0;
//         s1->data.sphere.rgb = (t_rgb){255, 255, 255};
//         s1->data.sphere.mat = material_metal((t_rgb){204.0, 204.0, 204.0}, 0.0);
//         obj_push(data->objects, s1);
//     }

//     // Left sphere: metal (0.8,0.8,0.8), fuzz 0.3
//     s3 = obj_new(SPHERE);
//     if (s3)
//     {
//         s3->data.sphere.cords = vec3_init_inline(-1.0, 0.0, -1.0);
//         s3->data.sphere.diameter = 1.0;
//         s3->data.sphere.rgb = (t_rgb){255, 255, 255};
//         s3->data.sphere.mat = material_metal((t_rgb){204.0, 153.0, 51.0}, 0.0);
//         obj_push(data->objects, s3);
//     }

//     // Right sphere: metal (0.8,0.6,0.2), fuzz 1.0
//     s4 = obj_new(SPHERE);
//     if (s4)
//     {
//         s4->data.sphere.cords = vec3_init_inline(1.0, 0.0, -1.0);
//         s4->data.sphere.diameter = 1.0;
//         s4->data.sphere.rgb = (t_rgb){255, 255, 255};
//         s4->data.sphere.mat = material_metal((t_rgb){204.0, 153.0, 51.0}, 0.0);
//         obj_push(data->objects, s4);
//     }
// }

int	get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	cleanup_objects(t_obj_list *objects)
{
	t_obj	*current;
	t_obj	*next;

	if (!objects)
		return ;
	current = objects->head;
	while (current)
	{
		next = current->next;
		if (current->type == SPHERE && current->data.sphere.mat)
			free(current->data.sphere.mat);
		if (current->type == PLANE && current->data.plane.mat)
			free(current->data.plane.mat);
		if (current->type == CYLINDER && current->data.cylinder.mat)
			free(current->data.cylinder.mat);
		free(current);
		current = next;
	}
	free(objects);
}

/**
 * Main entry point of the miniRT ray tracer
 * Initializes camera, creates demo world, sets up MLX window
 * Renders the scene and starts the main loop
 */
int	main(int ac, char **av)
{
	t_data	data;

	data.lights_on = false;
	(void)ac;
    if (parse_scene(av[1], &data))
        return (1);
    print_scene(&data);
    data.aa_state = ANTI_ALIASING;
	data.camera.samples_per_pixel = AA_MAX_SAMPLES;
	init_camera(&data);
	// build_demo_world(&data);
	mlx_set_setting(MLX_MAXIMIZED, false);
	data.mlx = mlx_init(WIDTH, HEIGHT, "miniRT", false);
	if (!data.mlx)
		return (1);
	data.img = mlx_new_image(data.mlx, WIDTH, HEIGHT);
	if (!data.img || (mlx_image_to_window(data.mlx, data.img, 0, 0) < 0))
		return (1);
    init_threads(&data);
	mlx_key_hook(data.mlx, key_hook, &data);
	render(&data);
	mlx_loop(data.mlx);
	cleanup_objects(data.objects);
	mlx_delete_image(data.mlx, data.img);
	mlx_terminate(data.mlx);
    cleanup_data(&data);
	return (0);
}
