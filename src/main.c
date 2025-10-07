/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 11:00:01 by apregitz          #+#    #+#             */
/*   Updated: 2025/10/02 11:43:11 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Creates a demo scene with two spheres for testing
 * Sets up object list and adds one small sphere and one large ground sphere
 * Used to test ray tracing functionality
 */
static void	build_demo_world(t_data *data)
{
	t_obj	*s1;
	t_obj	*s3;
	t_obj	*s4;
	t_obj	*p_floor;
	t_obj	*p_back;
	t_obj	*p_left;
	t_obj	*cyl;

	data->objects = (t_obj_list *)malloc(sizeof(t_obj_list));
	if (!data->objects)
		return;
	data->objects->head = NULL;
	data->objects->tail = NULL;
	data->objects->size = 0;

	s1 = obj_new(SPHERE);
	if (s1)
	{
		s1->data.sphere.cords = vec3_init_inline(0.0, 0.0, -1.2);
		s1->data.sphere.diameter = 1.0;
		s1->data.sphere.rgb = (t_rgb){255, 255, 255};
		s1->data.sphere.mat = material_metal((t_rgb){204.0, 204.0, 204.0}, 0.0);
		obj_push(data->objects, s1);
	}

	s3 = obj_new(SPHERE);
	if (s3)
	{
		s3->data.sphere.cords = vec3_init_inline(-1.0, 0.0, -1.0);
		s3->data.sphere.diameter = 1.0;
		s3->data.sphere.rgb = (t_rgb){255, 255, 255};
		s3->data.sphere.mat = material_metal((t_rgb){204.0, 153.0, 51.0}, 0.0);
		obj_push(data->objects, s3);
	}

	s4 = obj_new(SPHERE);
	if (s4)
	{
		s4->data.sphere.cords = vec3_init_inline(1.0, 0.0, -1.0);
		s4->data.sphere.diameter = 1.0;
		s4->data.sphere.rgb = (t_rgb){255, 255, 255};
		s4->data.sphere.mat = material_metal((t_rgb){204.0, 153.0, 51.0}, 0.0);
		obj_push(data->objects, s4);
	}

	p_floor = obj_new(PLANE);
	if (p_floor)
	{
		p_floor->data.plane.cords = vec3_init_inline(0.0, -0.5, 0.0);
		p_floor->data.plane.norm = vec3_init_inline(0.0, 1.0, 0.0);
		p_floor->data.plane.rgb = (t_rgb){200, 200, 200};
		p_floor->data.plane.mat = material_metal((t_rgb){200.0, 200.0, 200.0}, 0.0);
		obj_push(data->objects, p_floor);
	}

	p_back = obj_new(PLANE);
	if (p_back)
	{
		p_back->data.plane.cords = vec3_init_inline(0.0, 0.0, -5.0);
		p_back->data.plane.norm = vec3_init_inline(0.0, 0.0, 1.0);
		p_back->data.plane.rgb = (t_rgb){180, 180, 255};
		p_back->data.plane.mat = material_metal((t_rgb){180.0, 180.0, 255.0}, 0.0);
		obj_push(data->objects, p_back);
	}

	p_left = obj_new(PLANE);
	if (p_left)
	{
		p_left->data.plane.cords = vec3_init_inline(-2.0, 0.0, -1.0);
		p_left->data.plane.norm = vec3_init_inline(1.0, 0.0, 0.0);
		p_left->data.plane.rgb = (t_rgb){255, 200, 200};
		p_left->data.plane.mat = material_metal((t_rgb){255.0, 200.0, 200.0}, 0.0);
		obj_push(data->objects, p_left);
	}

	cyl = obj_new(CYLINDER);
	if (cyl)
	{
		cyl->data.cylinder.cords = vec3_init_inline(0.0, -0.25, -2.0);
		cyl->data.cylinder.norm = vec3_init_inline(0.0, 1.0, 0.0);
		cyl->data.cylinder.diameter = 0.6;
		cyl->data.cylinder.height = 6.2;
		cyl->data.cylinder.rgb = (t_rgb){200, 100, 50};
		cyl->data.cylinder.mat = material_metal((t_rgb){200.0, 100.0, 50.0}, 0.0);
		obj_push(data->objects, cyl);
	}
}

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
	int	render_time;

	(void)ac;
	(void)av;
	data.aa_state = ANTI_ALIASING;
	data.camera.samples_per_pixel = AA_MAX_SAMPLES;
	init_camera(&data);
	build_demo_world(&data);
	mlx_set_setting(MLX_MAXIMIZED, false);
	data.mlx = mlx_init(WIDTH, HEIGHT, "miniRT", false);
	if (!data.mlx)
		return (1);
	data.img = mlx_new_image(data.mlx, WIDTH, HEIGHT);
	if (!data.img || (mlx_image_to_window(data.mlx, data.img, 0, 0) < 0))
		return (1);
	mlx_key_hook(data.mlx, key_hook, &data);
	render_time = get_time_in_ms();
	render(&data);
	printf("\n%.2f sec\n", (float)(get_time_in_ms() - render_time) / 1000);
	mlx_loop(data.mlx);
	cleanup_objects(data.objects);
	mlx_delete_image(data.mlx, data.img);
	mlx_terminate(data.mlx);
	return (0);
}
