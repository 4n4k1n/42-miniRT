/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bump.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 14:59:27 by nweber            #+#    #+#             */
/*   Updated: 2025/10/28 14:59:53 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Parses a bump token in the form "bm:<path>[:strength]"
 * Loads the PNG, copies pixels, and assigns bump to the given object.
 * Returns 0 on success, 1 on failure.
 */
int	parse_bump(const char *token, t_obj *o)
{
	const char		*rest;
	const char		*sep;
	char			*path;
	double			strength;
	mlx_texture_t	*tex;
	size_t			sz;
	t_bump			*b;

	if (!token || ft_strncmp(token, "bm:", 3) != 0 || !o)
		return (1);
	rest = token + 3;
	sep = ft_strchr(rest, ':');
	if (sep)
	{
		path = ft_substr(rest, 0, (size_t)(sep - rest));
		if (!path)
			return (1);
		strength = ft_atof(sep + 1);
		if (strength <= 0.0)
			strength = 1.0;
	}
	else
	{
		path = ft_strdup(rest);
		if (!path)
			return (1);
		strength = 1.0;
	}
	tex = mlx_load_png(path);
	free(path);
	if (!tex || !tex->pixels || !tex->width || !tex->height)
		return (ft_error("bump png load", 1));
	b = (t_bump *)malloc(sizeof(t_bump));
	if (!b)
	{
		if (tex)
		{
			free(tex->pixels);
			free(tex);
		}
		return (ft_error("malloc", 1));
	}
	sz = (size_t)tex->width * (size_t)tex->height * (size_t)tex->bytes_per_pixel;
	b->pixels = (unsigned char *)malloc(sz);
	if (!b->pixels)
	{
		free(b);
		free(tex->pixels);
		free(tex);
		return (ft_error("malloc", 1));
	}
	ft_memcpy(b->pixels, tex->pixels, sz);
	b->width = tex->width;
	b->height = tex->height;
	b->strength = strength;
	free(tex->pixels);
	free(tex);
	if (o->type == SPHERE)
		o->data.sphere.bump = b;
	else if (o->type == PLANE)
		o->data.plane.bump = b;
	else if (o->type == CYLINDER)
		o->data.cylinder.bump = b;
	else if (o->type == PYRAMID)
		o->data.pyramid.bump = b;
	else if (o->type == CONE)
		o->data.cone.bump = b;
	else
	{
		free(b->pixels);
		free(b);
		return (1);
	}
	return (0);
}
