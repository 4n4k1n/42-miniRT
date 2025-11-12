/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_cylinder.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 13:50:03 by nweber            #+#    #+#             */
/*   Updated: 2025/11/12 12:31:45 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Parses required cylinder fields and allocates the object.
 * Indices: 1=pos, 2=axis, 3=diameter, 4=height, 5=rgb.
 * Axis must be non-zero and in [-1,1]. Sets bump = NULL.
 * @param tokens token array (cy ...)
 * @param out receives allocated cylinder object
 * @return 0 success, 1 error
 */
static int	parse_cyl_core(char **tokens, t_obj **out)
{
	t_obj	*o;

	*out = NULL;
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
	o->data.cylinder.bump = NULL;
	*out = o;
	return (0);
}

/**
 * Parses optional material and bump tokens.
 * Bump must be last: "bm:<path>[:strength]".
 * Decrements len if bump is consumed, then parses material.
 * @param tokens token array
 * @param len in/out token count (may be decremented)
 * @param o cylinder to update
 * @return 0 success, 1 error
 */
static int	parse_cyl_extras(char **tokens, int *len, t_obj *o)
{
	if (*len >= 7 && ft_strncmp(tokens[*len - 1], "bm:", 3) == 0)
	{
		if (parse_bump(tokens[*len - 1], o))
			return (rt_error("invalid cylinder bump"));
		(*len)--;
	}
	if (parse_material(tokens, *len, o))
		return (rt_error("invalid cylinder material"));
	return (0);
}

/**
 * Parses cylinder object parameters from tokens
 * Format: cy <x,y,z> <nx,ny,nz> <diameter> <height> <r,g,b>
 * [MaterialToken] [bm:<path>[:strength]]
 * The bump token, if present, must be last.
 */
int	parse_cylinder(char **tokens, t_data *scene)
{
	t_obj	*o;
	int		len;

	o = NULL;
	len = ft_arrlen(tokens);
	if (len < 6 || len > 8)
		return (rt_error("invalid cylinder format"));
	if (parse_cyl_core(tokens, &o))
		return (1);
	if (parse_cyl_extras(tokens, &len, o))
		return (free(o), 1);
	if (obj_push(scene->objects, o))
		return (free(o), rt_error("object push failed"));
	return (0);
}
