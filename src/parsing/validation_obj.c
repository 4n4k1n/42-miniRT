/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation_obj.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 10:36:19 by nweber            #+#    #+#             */
/*   Updated: 2025/09/30 10:36:33 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	parse_sphere(char **tok, t_main *scene)
{
	t_obj	*o;

	if (ft_arrlen((void **)tok) != 4)
		return (rt_error("invalid sphere format"));
	o = obj_new(SPHERE);
	if (!o)
		return (rt_error("malloc failed (sphere)"));
	if (parse_vec3(tok[1], &o->data.sphere.cords))
		return (free(o), rt_error("invalid sphere center"));
	if (parse_double(tok[2], &o->data.sphere.diameter) || o->data.sphere.diameter <= 0.0)
		return (free(o), rt_error("invalid sphere diameter"));
	if (parse_rgb(tok[3], &o->data.sphere.rgb))
		return (free(o), rt_error("invalid sphere RGB"));
	if (obj_push(scene->objects, o))
		return (free(o), rt_error("object push failed"));
	return (0);
}

int	parse_plane(char **tok, t_main *scene)
{
	t_obj	*o;

	if (ft_arrlen((void **)tok) != 4)
		return (rt_error("invalid plane format"));
	o = obj_new(PLANE);
	if (!o)
		return (rt_error("malloc failed (plane)"));
	if (parse_vec3(tok[1], &o->data.plane.cords))
		return (free(o), rt_error("invalid plane point"));
	if (parse_vec3(tok[2], &o->data.plane.norm) || !vec_in_range(o->data.plane.norm, -1.0, 1.0)
		|| !vec_non_zero(o->data.plane.norm))
		return (free(o), rt_error("invalid plane normal"));
	if (parse_rgb(tok[3], &o->data.plane.rgb))
		return (free(o), rt_error("invalid plane RGB"));
	if (obj_push(scene->objects, o))
		return (free(o), rt_error("object push failed"));
	return (0);
}

int	parse_cylinder(char **tok, t_main *scene)
{
	t_obj	*o;

	if (ft_arrlen((void **)tok) != 6)
		return (rt_error("invalid cylinder format"));
	o = obj_new(CYLINDER);
	if (!o)
		return (rt_error("malloc failed (cylinder)"));
	if (parse_vec3(tok[1], &o->data.cylinder.cords))
		return (free(o), rt_error("invalid cylinder center"));
	if (parse_vec3(tok[2], &o->data.cylinder.norm) || !vec_in_range(o->data.cylinder.norm, -1.0, 1.0)
		|| !vec_non_zero(o->data.cylinder.norm))
		return (free(o), rt_error("invalid cylinder axis"));
	if (parse_double(tok[3], &o->data.cylinder.diameter) || o->data.cylinder.diameter <= 0.0)
		return (free(o), rt_error("invalid cylinder diameter"));
	if (parse_double(tok[4], &o->data.cylinder.height) || o->data.cylinder.height <= 0.0)
		return (free(o), rt_error("invalid cylinder height"));
	if (parse_rgb(tok[5], &o->data.cylinder.rgb))
		return (free(o), rt_error("invalid cylinder RGB"));
	if (obj_push(scene->objects, o))
		return (free(o), rt_error("object push failed"));
	return (0);
}
