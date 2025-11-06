/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_pyramid.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 13:50:04 by nweber            #+#    #+#             */
/*   Updated: 2025/11/06 13:43:06 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Parses pyramid object parameters from tokens
 * Format: py <x,y,z> <nx,ny,nz> <base> <height> <r,g,b>
 * [MaterialToken] [bm:<path>[:strength]]
 * The bump token, if present, must be last.
 */
int	parse_pyramid(char **tokens, t_data *scene)
{
	t_obj	*o;
	int		len;

	len = ft_arrlen(tokens);
	if (len < 6 || len > 8)
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
	o->data.pyramid.bump = NULL;
	if (len >= 7 && ft_strncmp(tokens[len - 1], "bm:", 3) == 0)
	{
		if (parse_bump(tokens[len - 1], o))
			return (free(o), rt_error("invalid pyramid bump"));
		len--;
	}
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
