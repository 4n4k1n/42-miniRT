/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_cone.c                                      :+:      :+:    :+:  */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 13:50:04 by nweber            #+#    #+#             */
/*   Updated: 2025/11/06 13:50:04 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static int	parse_cone_core(char **tokens, t_obj **out)
{
	t_obj	*o;

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
	o->data.cone.bump = NULL;
	*out = o;
	return (0);
}

static int	parse_cone_extras(char **tokens, int *len, t_obj *o)
{
	if (*len >= 7 && ft_strncmp(tokens[*len - 1], "bm:", 3) == 0)
	{
		if (parse_bump(tokens[*len - 1], o))
			return (rt_error("invalid cone bump"));
		(*len)--;
	}
	if (parse_material(tokens, *len, o))
		return (rt_error("invalid cone material"));
	return (0);
}

/**
 * Parses cone object parameters from tokens
 * Format: co <x,y,z> <nx,ny,nz> <diameter> <height> <r,g,b>
 * [MaterialToken] [bm:<path>[:strength]]
 * The bump token, if present, must be last.
 */
int	parse_cone(char **tokens, t_data *scene)
{
	t_obj	*o;
	int		len;

	o = NULL;
	len = ft_arrlen(tokens);
	if (len < 6 || len > 8)
		return (rt_error("invalid cone format"));
	if (parse_cone_core(tokens, &o))
		return (1);
	if (parse_cone_extras(tokens, &len, o))
		return (free(o), 1);
	if (obj_push(scene->objects, o))
		return (free(o), rt_error("object push failed"));
	return (0);
}
