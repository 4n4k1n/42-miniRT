/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 20:20:31 by nweber            #+#    #+#             */
/*   Updated: 2025/09/30 11:32:27 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

int	parse_ambient(char **tokens, t_data *scene, t_arg_check *args)
{
	double	ratio;

	if (args->has_a)
		return (rt_error("duplicate ambient"));
	if (ft_arrlen(tokens) != 3)
		return (rt_error("invalid ambient format"));
	if (parse_double(tokens[1], &ratio))
		return (rt_error("invalid ambient ratio"));
	if (!in_range_d(ratio, 0.0, 1.0))
		return (rt_error("ambient ratio out of range [0,1]"));
	if (parse_rgb(tokens[2], &scene->ambiente.color))
		return (rt_error("invalid ambient RGB"));
	scene->ambiente.lighting = ratio;
	args->has_a = 1;
	return (0);
}

int	parse_camera(char **tokens, t_data *scene, t_arg_check *args)
{
	t_vec3	pos;
	t_vec3	dir;
	double	fov;

	if (args->has_c)
		return (rt_error("duplicate camera (C)"));
	if (ft_arrlen(tokens) != 4)
		return (rt_error("invalid camera format"));
	if (parse_vec3(tokens[1], &pos) || parse_vec3(tokens[2], &dir))
		return (rt_error("invalid camera vector"));
	if (!vec_in_range(&dir, -1.0, 1.0))
		return (rt_error("camera orientation out of range [-1,1]"));
	if (parse_double(tokens[3], &fov) || !in_range_d(fov, 0.0, 180.0))
		return (rt_error("camera FOV out of range [0,180]"));
	scene->camera.cords = pos;
	scene->camera.orientation = dir;
	scene->camera.fov = fov;
	args->has_c = 1;
	return (0);
}

int	parse_light(char **tokens, t_data *scene, t_arg_check *args)
{
	int		argc;
	t_light	*ln;

	if (args->has_l)
		return (rt_error("duplicate light (L)"));
	argc = (int)ft_arrlen(tokens);
	if (argc != 3 && argc != 4)
		return (rt_error("invalid light format"));
	ln = light_new();
	if (!ln)
		return (rt_error("malloc failed (light)"));
	if (parse_vec3(tokens[1], &ln->cords))
		return (free(ln), rt_error("invalid light position"));
	if (parse_double(tokens[2], &ln->color.r))
		return (free(ln), rt_error("invalid light brightness"));
	if (!in_range_d(ln->color.r, 0.0, 1.0))
		return (free(ln), rt_error("light brightness out of range [0,1]"));
	if (argc == 4)
	{
		if (parse_rgb(tokens[3], &ln->color))
			return (free(ln), rt_error("invalid light RGB"));
	}
	else
	{
		ln->color.r = 255.0;
		ln->color.g = 255.0;
		ln->color.b = 255.0;
	}
	if (light_push(scene->light_list, ln))
		return (free(ln), rt_error("light push failed"));
	args->has_l = 1;
	return (0);
}
