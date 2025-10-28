/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation_obj.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 10:36:19 by nweber            #+#    #+#             */
/*   Updated: 2025/10/28 11:29:13 by nweber           ###   ########.fr       */
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

int	parse_pyramid(char **tokens, t_data *scene)
{
	t_obj	*o;
	int		len;

	len = ft_arrlen(tokens);
	if (len != 6 && len != 7)
		return (rt_error("invalid pyramid format"));
	o = obj_new(PYRAMID);
	if (!o)
		return (rt_error("malloc failed (pyramid)"));
	if (parse_vec3(tokens[1], &o->data.pyramid.cords))
		return (free(o), rt_error("invalid pyramid center"));
	if (parse_vec3(tokens[2], &o->data.pyramid.norm)
		|| !vec_in_range(&o->data.pyramid.norm, -1.0, 1.0)
		|| !vec_non_zero(o->data.pyramid.norm))
		return (free(o), rt_error("invalid pyramid axis"));
	if (parse_double(tokens[3], &o->data.pyramid.base)
		|| o->data.pyramid.base <= 0.0)
		return (free(o), rt_error("invalid pyramid base"));
	if (parse_double(tokens[4], &o->data.pyramid.height)
		|| o->data.pyramid.height <= 0.0)
		return (free(o), rt_error("invalid pyramid height"));
	if (parse_rgb(tokens[5], &o->data.pyramid.rgb))
		return (free(o), rt_error("invalid pyramid RGB"));
	if (parse_material(tokens, len, o))
		return (free(o), rt_error("invalid pyramid material"));
	if (obj_push(scene->objects, o))
	{
		if (o->data.pyramid.mat)
			free(o->data.pyramid.mat);
		return (free(o), rt_error("obj push failed"));
	}
	return (0);
}

int	parse_cone(char **tokens, t_data *scene)
{
	t_obj	*o;
	int		len;

	len = ft_arrlen(tokens);
	if (len != 6 && len != 7)
		return (rt_error("invalid cone format"));
	o = obj_new(CONE);
	if (!o)
		return (rt_error("malloc failed (cone)"));
	if (parse_vec3(tokens[1], &o->data.cone.cords))
		return (free(o), rt_error("invalid cone center"));
	if (parse_vec3(tokens[2], &o->data.cone.norm)
		|| !vec_in_range(&o->data.cone.norm, -1.0, 1.0)
		|| !vec_non_zero(o->data.cone.norm))
		return (free(o), rt_error("invalid cone axis"));
	if (parse_double(tokens[3], &o->data.cone.diameter)
		|| o->data.cone.diameter <= 0.0)
		return (free(o), rt_error("invalid cone diameter"));
	if (parse_double(tokens[4], &o->data.cone.height)
		|| o->data.cone.height <= 0.0)
		return (free(o), rt_error("invalid cone height"));
	if (parse_rgb(tokens[5], &o->data.cone.rgb))
		return (free(o), rt_error("invalid cone RGB"));
	if (parse_material(tokens, len, o))
		return (free(o), rt_error("invalid cone material"));
	if (obj_push(scene->objects, o))
	{
		if (o->data.cone.mat)
			free(o->data.cone.mat);
		return (free(o), rt_error("object push failed"));
	}
	return (0);
}

/**
 * Parses triangle object parameters from tokens
 * Format: tr <x1,y1,z1> <x2,y2,z2> <x3,y3,z3> <r,g,b>
 * @param tokens Array of parsed string tokens
 * @param scene Scene data structure to populate
 * @return 0 on success, 1 on failure
 */
int	parse_triangle(char **tokens, t_data *scene)
{
	t_obj	*o;
	int		len;

	len = ft_arrlen(tokens);
	if (len != 5 && len != 6)
		return (rt_error("invalid triangle format"));
	o = obj_new(TRIANGLE);
	if (!o)
		return (rt_error("malloc failed (triangle)"));
	if (parse_vec3(tokens[1], &o->data.triangle.v0))
		return (free(o), rt_error("invalid triangle vertex 0"));
	if (parse_vec3(tokens[2], &o->data.triangle.v1))
		return (free(o), rt_error("invalid triangle vertex 1"));
	if (parse_vec3(tokens[3], &o->data.triangle.v2))
		return (free(o), rt_error("invalid triangle vertex 2"));
	if (parse_rgb(tokens[4], &o->data.triangle.rgb))
		return (free(o), rt_error("invalid triangle RGB"));
	if (parse_material(tokens, len, o))
		return (free(o), rt_error("invalid triangle material"));
	if (obj_push(scene->objects, o))
	{
		if (o->data.triangle.mat)
			free(o->data.triangle.mat);
		return (free(o), rt_error("object push failed"));
	}
	return (0);
}
