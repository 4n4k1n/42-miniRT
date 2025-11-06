/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_sphere.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 13:50:01 by nweber            #+#    #+#             */
/*   Updated: 2025/11/06 13:42:44 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Parses sphere object parameters from tokens
 * Format: sp <x,y,z> <diameter> <r,g,b> [MaterialToken] [bm:<path>[:strength]]
 * The bump token, if present, must be last.
 */
int	parse_sphere(char **tokens, t_data *scene)
{
	t_obj	*o;
	int		len;

	len = ft_arrlen(tokens);
	if (len < 4 || len > 6)
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
	o->data.sphere.bump = NULL;
	if (len >= 5 && ft_strncmp(tokens[len - 1], "bm:", 3) == 0)
	{
		if (parse_bump(tokens[len - 1], o))
			return (free(o), rt_error("invalid sphere bump"));
		len--;
	}
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
