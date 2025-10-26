/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation_obj.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 10:36:19 by nweber            #+#    #+#             */
/*   Updated: 2025/10/05 12:21:58 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Parses sphere object parameters from tokens
 * Format: sp <x,y,z> <diameter> <r,g,b>
 * @param tokens Array of parsed string tokens
 * @param scene Scene data structure to populate
 * @return 0 on success, 1 on failure
 */
int	parse_sphere(char **tokens, t_data *scene)
{
	t_obj	*o;
	int		len;

	len = ft_arrlen(tokens);
	if (len != 4 && len != 5)
		return (rt_error("invalid sphere format"));
	o = obj_new(SPHERE);
	if (!o)
		return (rt_error("malloc failed (sphere)"));
	if (parse_vec3(tokens[1], &o->data.sphere.cords))
		return (free(o), rt_error("invalid sphere center"));
	if (parse_double(tokens[2], &o->data.sphere.diameter)
		|| o->data.sphere.diameter <= 0.0)
		return (free(o), rt_error("invalid sphere diameter"));
	if (parse_rgb(tokens[3], &o->data.sphere.rgb))
		return (free(o), rt_error("invalid sphere RGB"));
	if (parse_material(tokens, len, o))
		return (free(o), rt_error("invalid sphere material"));
	if (obj_push(scene->objects, o))
	{
		if (o->data.sphere.mat)
			free(o->data.sphere.mat);
		return (free(o), rt_error("object push failed"));
	}
	return (0);
}

/**
 * Parses plane object parameters from tokens
 * Format: pl <x,y,z> <nx,ny,nz> <r,g,b>
 * @param tokens Array of parsed string tokens
 * @param scene Scene data structure to populate
 * @return 0 on success, 1 on failure
 */
int	parse_plane(char **tokens, t_data *scene)
{
	t_obj	*o;
	int		len;

	len = ft_arrlen(tokens);
	if (len != 4 && len != 5)
		return (rt_error("invalid plane format"));
	o = obj_new(PLANE);
	if (!o)
		return (rt_error("malloc failed (plane)"));
	if (parse_vec3(tokens[1], &o->data.plane.cords))
		return (free(o), rt_error("invalid plane point"));
	if (parse_vec3(tokens[2], &o->data.plane.norm)
		|| !vec_in_range(&o->data.plane.norm, -1.0, 1.0)
		|| !vec_non_zero(o->data.plane.norm))
		return (free(o), rt_error("invalid plane normal"));
	if (parse_rgb(tokens[3], &o->data.plane.rgb))
		return (free(o), rt_error("invalid plane RGB"));
	if (parse_material(tokens, len, o))
		return (free(o), rt_error("invalid plane material"));
	if (obj_push(scene->objects, o))
		return (free(o), rt_error("object push failed"));
	return (0);
}

/**
 * Parses cylinder object parameters from tokens
 * Format: cy <x,y,z> <nx,ny,nz> <diameter> <height> <r,g,b>
 * @param tokens Array of parsed string tokens
 * @param scene Scene data structure to populate
 * @return 0 on success, 1 on failure
 */
int	parse_cylinder(char **tokens, t_data *scene)
{
	t_obj	*o;
	int		len;

	len = ft_arrlen(tokens);
	if (len != 6 && len != 7)
		return (rt_error("invalid cylinder format"));
	o = obj_new(CYLINDER);
	if (!o)
		return (rt_error("malloc failed (cylinder)"));
	if (parse_vec3(tokens[1], &o->data.cylinder.cords))
		return (free(o), rt_error("invalid cylinder center"));
	if (parse_vec3(tokens[2], &o->data.cylinder.norm)
		|| !vec_in_range(&o->data.cylinder.norm, -1.0, 1.0)
		|| !vec_non_zero(o->data.cylinder.norm))
		return (free(o), rt_error("invalid cylinder axis"));
	if (parse_double(tokens[3], &o->data.cylinder.diameter)
		|| o->data.cylinder.diameter <= 0.0)
		return (free(o), rt_error("invalid cylinder diameter"));
	if (parse_double(tokens[4], &o->data.cylinder.height)
		|| o->data.cylinder.height <= 0.0)
		return (free(o), rt_error("invalid cylinder height"));
	if (parse_rgb(tokens[5], &o->data.cylinder.rgb))
		return (free(o), rt_error("invalid cylinder RGB"));
	if (parse_material(tokens, len, o))
		return (free(o), rt_error("invalid cylinder material"));
	if (obj_push(scene->objects, o))
		return (free(o), rt_error("object push failed"));
	return (0);
}
