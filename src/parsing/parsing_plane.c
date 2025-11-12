/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_plane.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 13:50:02 by nweber            #+#    #+#             */
/*   Updated: 2025/11/12 12:32:41 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Parses required plane fields and allocates the object.
 * Indices: 1=point, 2=normal, 3=rgb.
 * Normal must be non-zero and in [-1,1]. Sets bump = NULL.
 * @param tokens token array (pl ...)
 * @param out receives allocated plane object
 * @return 0 success, 1 error
 */
static int	parse_plane_core(char **tokens, t_obj **out)
{
	t_obj	*o;

	*out = NULL;
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
	o->data.plane.bump = NULL;
	*out = o;
	return (0);
}

/**
 * Parses optional material and bump tokens.
 * Bump must be last: "bm:<path>[:strength]".
 * Decrements len if bump is consumed, then parses material.
 * @param tokens token array
 * @param len in/out token count (may be decremented)
 * @param o plane to update
 * @return 0 success, 1 error
 */
static int	parse_plane_extras(char **tokens, int *len, t_obj *o)
{
	if (*len >= 5 && ft_strncmp(tokens[*len - 1], "bm:", 3) == 0)
	{
		if (parse_bump(tokens[*len - 1], o))
			return (rt_error("invalid plane bump"));
		(*len)--;
	}
	if (parse_material(tokens, *len, o))
		return (rt_error("invalid plane material"));
	return (0);
}

/**
 * Parses plane object parameters from tokens
 * Format: pl <x,y,z> <nx,ny,nz> <r,g,b> [MaterialToken] [bm:<path>[:strength]]
 * The bump token, if present, must be last.
 */
int	parse_plane(char **tokens, t_data *scene)
{
	t_obj	*o;
	int		len;

	o = NULL;
	len = ft_arrlen(tokens);
	if (len < 4 || len > 6)
		return (rt_error("invalid plane format"));
	if (parse_plane_core(tokens, &o))
		return (1);
	if (parse_plane_extras(tokens, &len, o))
		return (free(o), 1);
	if (obj_push(scene->objects, o))
		return (free(o), rt_error("object push failed"));
	return (0);
}
