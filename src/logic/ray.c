/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 10:52:20 by anakin            #+#    #+#             */
/*   Updated: 2025/11/15 15:48:51 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Traces the current ray and writes vars->rec on hit.
 * Uses BVH when enabled, otherwise brute-force world_hit.
 * @param data global scene data
 * @param vars ray-color working state (reads current_ray, writes rec)
 * @return 1 if a hit is found, 0 otherwise
 */
static int	check_hit(t_data *data, t_ray_color_vars *vars)
{
	t_bvh_ctx	bvh_ctx;
	t_hit_range	range;

	range.tmin = 0.001;
	range.tmax = INFINITY;
	if (data->settings.use_bvh)
	{
		bvh_ctx = (t_bvh_ctx){data->bvh_root, data->objects,
			&vars->current_ray, range};
		if (world_hit_bvh(&bvh_ctx, &vars->rec))
			return (1);
	}
	if (!data->settings.use_bvh && data->objects
		&& world_hit(data->objects, &vars->current_ray, range, &vars->rec))
		return (1);
	return (0);
}

/**
 * Processes material scatter at the hit point.
 * Updates throughput and current_ray; applies Russian roulette for depth >= 3.
 * @param data global scene data
 * @param vars ray-color state (uses rec, updates current_ray/throughput/depth)
 * @return 1 to continue path tracing, 0 to terminate
 */
static int	process_scatter(t_data *data, t_ray_color_vars *vars)
{
	struct s_scatter_ctx	ctx;

	(void)data;
	if (!vars->rec.mat)
		return (0);
	ctx.r_in = &vars->current_ray;
	ctx.rec = &vars->rec;
	ctx.attenuation = &vars->attenuation;
	ctx.scattered = &vars->scattered;
	if (!vars->rec.mat->scatter(vars->rec.mat, &ctx))
		return (0);
	vars->direct_contrib = rgb_modulate(vars->throughput,
			vars->direct_light);
	vars->final_color = rgb_add(vars->final_color,
			vars->direct_contrib);
	vars->throughput = rgb_modulate(vars->throughput,
			vars->attenuation);
	if (vars->depth >= 3)
	{
		if (russian_roulette(vars))
			return (0);
	}
	vars->current_ray = vars->scattered;
	vars->depth++;
	return (1);
}

/**
 * Handles shading when a hit occurs.
 * Applies bump, computes direct light on primary, and either scatters
 * or accumulates remaining direct light.
 * @param data global scene data
 * @param vars ray-color state for the current path
 * @return final accumulated color, or {-1,-1,-1} to indicate continue
 */
static t_rgb	process_hit(t_data *data, t_ray_color_vars *vars)
{
	t_rgb	simple_shading;

	if (!vars->rec.mat)
	{
		simple_shading = calculate_direct_lighting(data, &vars->rec);
		simple_shading = rgb_add(simple_shading,
				get_ambient_light(data, &vars->rec));
		vars->final_color = rgb_add(vars->final_color,
				rgb_modulate(vars->throughput, simple_shading));
		return (vars->final_color);
	}
	if (vars->rec.bump)
		apply_bump_mapping(vars);
	if (vars->depth == 0)
		vars->direct_light = calculate_direct_lighting(data, &vars->rec);
	if (process_scatter(data, vars))
		return ((t_rgb){-1.0, -1.0, -1.0});
	vars->final_color = rgb_add(vars->final_color,
			rgb_modulate(vars->throughput, vars->direct_light));
	return (vars->final_color);
}

/**
 * Computes the path-traced color for a given ray.
 * Iteratively traces bounces up to max_depth, accumulating contributions.
 * @param initial_ray input camera or scattered ray
 * @param data global scene data
 * @param max_depth maximum bounce count
 * @return final accumulated RGB color
 */
t_rgb	ray_color(t_ray *initial_ray, t_data *data, int max_depth)
{
	t_ray_color_vars	vars;
	t_rgb				result;

	vars.current_ray = *initial_ray;
	vars.final_color = (t_rgb){0.0, 0.0, 0.0};
	vars.throughput = (t_rgb){255.0, 255.0, 255.0};
	vars.depth = 0;
	while (vars.depth < max_depth)
	{
		if (check_hit(data, &vars))
		{
			result = process_hit(data, &vars);
			if (result.r >= 0.0)
				return (result);
			continue ;
		}
		vars.sky = calculate_final_color(&vars.final_color,
				&vars.current_ray);
		vars.final_color = rgb_add(vars.final_color,
				rgb_modulate(vars.throughput, vars.sky));
		return (vars.final_color);
	}
	return (vars.final_color);
}
