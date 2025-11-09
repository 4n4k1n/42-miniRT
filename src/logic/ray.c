/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 10:52:20 by anakin            #+#    #+#             */
/*   Updated: 2025/11/07 13:17:00 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static int	check_hit(t_data *data, t_ray_color_vars *vars)
{
	if (data->settings.use_bvh && world_hit_bvh(data->bvh_root,
			data->objects, &vars->current_ray, 0.001, INFINITY,
			&vars->rec))
		return (1);
	if (!data->settings.use_bvh && data->objects
		&& world_hit(data->objects, &vars->current_ray, 0.001, INFINITY,
			&vars->rec))
		return (1);
	return (0);
}

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

static t_rgb	process_hit(t_data *data, t_ray_color_vars *vars)
{
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
