/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_triangle.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 13:50:05 by nweber            #+#    #+#             */
/*   Updated: 2025/11/06 13:50:05 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

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
