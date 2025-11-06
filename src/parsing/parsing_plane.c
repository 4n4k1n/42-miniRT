/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_plane.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 13:50:02 by nweber            #+#    #+#             */
/*   Updated: 2025/11/06 13:43:11 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Parses plane object parameters from tokens
 * Format: pl <x,y,z> <nx,ny,nz> <r,g,b> [MaterialToken] [bm:<path>[:strength]]
 * The bump token, if present, must be last.
 */
int	parse_plane(char **tokens, t_data *scene)
{
	t_obj	*o;
	int		len;

	len = ft_arrlen(tokens);
	if (len < 4 || len > 6)
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
	o->data.plane.bump = NULL;
	if (len >= 5 && ft_strncmp(tokens[len - 1], "bm:", 3) == 0)
	{
		if (parse_bump(tokens[len - 1], o))
			return (free(o), rt_error("invalid plane bump"));
		len--;
	}
	if (parse_material(tokens, len, o))
		return (free(o), rt_error("invalid plane material"));
	if (obj_push(scene->objects, o))
		return (free(o), rt_error("object push failed"));
	return (0);
}
