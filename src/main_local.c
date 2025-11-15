/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_local.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 17:30:00 by anakin            #+#    #+#             */
/*   Updated: 2025/11/15 13:45:04 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Initialize runtime data for local (non-distributed) execution.
 * - Translates settings to runtime defines
 * - Parses the scene file into data
 * - Optionally builds BVH for non-plane objects
 * - Initializes camera and AA settings
 * @param data pointer to t_data to populate
 * @param scene_file path to the scene description file
 * @return 0 on success, non-zero on failure
 */
static int	init_data_local(t_data *data, char *scene_file)
{
	translate_settings(data);
	data->settings.light_state = false;
	data->settings.use_bvh = USE_BVH;
	data->bvh_root = NULL;
	if (parse_scene(scene_file, data))
		return (1);
	if (data->settings.use_bvh && data->objects && data->objects->size > 0)
	{
		data->bvh_root = build_bvh(data->objects);
		printf("BVH built with %zu objects\n", data->objects->size);
	}
	data->settings.aa_state = ANTI_ALIASING;
	data->camera.samples_per_pixel = AA_MIN_SAMPLES * 8;
	init_camera(data);
	return (0);
}

/**
 * Initialize the MLX context and create the window + image for rendering.
 * On failure this function also frees parsed scene resources.
 * @param data application data containing desired image dimensions
 * @return 0 on success, non-zero on failure
 */
static int	init_mlx_local(t_data *data)
{
	mlx_set_setting(MLX_MAXIMIZED, false);
	data->mlx = mlx_init(data->defines.width,
			data->defines.height, "miniRT", false);
	if (!data->mlx)
	{
		cleanup_objects(data->objects);
		cleanup_lights(data->light_list);
		return (1);
	}
	data->img = mlx_new_image(data->mlx, data->defines.width,
			data->defines.height);
	if (!data->img || (mlx_image_to_window(data->mlx, data->img, 0, 0) < 0))
	{
		cleanup_objects(data->objects);
		cleanup_lights(data->light_list);
		mlx_terminate(data->mlx);
		return (1);
	}
	return (0);
}

/**
 * Cleanup routine for local execution path.
 * Frees BVH, object and light lists, destroys MLX image/window/context and
 * shuts down thread pool / synchronization primitives.
 * @param data application data to clean up
 */
static void	cleanup_local(t_data *data)
{
	if (data->bvh_root)
		free_bvh(data->bvh_root);
	cleanup_objects(data->objects);
	cleanup_lights(data->light_list);
	mlx_delete_image(data->mlx, data->img);
	mlx_terminate(data->mlx);
	cleanup_data(data);
}

/**
 * Run the renderer in local single-machine mode.
 * Initializes scene and MLX, spawns worker threads, installs input hooks,
 * triggers the initial render and enters the MLX event loop. Cleans up on exit.
 * @param scene_file path to the scene description file
 * @return 0 on success, non-zero on failure
 */
int	run_local(char *scene_file)
{
	t_data	data;

	ft_memset(&data, 0, sizeof(data));
	if (init_data_local(&data, scene_file))
		return (1);
	if (init_mlx_local(&data))
		return (1);
	init_threads(&data);
	mlx_key_hook(data.mlx, key_hook, &data);
	render(&data);
	mlx_loop(data.mlx);
	cleanup_local(&data);
	return (0);
}
