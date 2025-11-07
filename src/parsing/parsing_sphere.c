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

static int	parse_sphere_core(char **tokens, t_obj **out)
{
	t_obj	*o;

	*out = NULL;
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
	*out = o;
	return (0);
}

static int	parse_sphere_extras(char **tokens, int *len, t_obj *o)
{
	if (*len >= 5 && ft_strncmp(tokens[*len - 1], "bm:", 3) == 0)
	{
		if (parse_bump(tokens[*len - 1], o))
			return (rt_error("invalid sphere bump"));
		(*len)--;
	}
	if (parse_material(tokens, *len, o))
		return (rt_error("invalid sphere material"));
	return (0);
}

/**
 * Parses sphere object parameters from tokens
 * Format: sp <x,y,z> <diameter> <r,g,b> [MaterialToken] [bm:<path>[:strength]]
 * The bump token, if present, must be last.
 */
int	parse_sphere(char **tokens, t_data *scene)
{
	t_obj	*o;
	int		len;

	o = NULL;
	len = ft_arrlen(tokens);
	if (len < 4 || len > 6)
		return (rt_error("invalid sphere format"));
	if (parse_sphere_core(tokens, &o))
		return (1);
	if (parse_sphere_extras(tokens, &len, o))
		return (free(o), 1);
	if (obj_push(scene->objects, o))
	{
		if (o->data.sphere.mat)
			free(o->data.sphere.mat);
		return (free(o), rt_error("object push failed"));
	}
	return (0);
}
