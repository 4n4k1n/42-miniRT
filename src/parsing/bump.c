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
 * Parse path and strength from bump token
 */
static int	parse_bump_token(const char *token, char **path, double *strength)
{
	const char	*rest;
	const char	*sep;

	rest = token + 3;
	sep = ft_strchr(rest, ':');
	if (sep)
	{
		*path = ft_substr(rest, 0, (size_t)(sep - rest));
		if (!*path)
			return (1);
		*strength = ft_atof(sep + 1);
		if (*strength <= 0.0)
			*strength = 1.0;
	}
	else
	{
		*path = ft_strdup(rest);
		if (!*path)
			return (1);
		*strength = 1.0;
	}
	return (0);
}

/**
 * Loads texture and creates bump structure
 */
static t_bump	*create_bump_from_tex(mlx_texture_t *tex, double strength)
{
	t_bump	*b;
	size_t	sz;

	b = (t_bump *)malloc(sizeof(t_bump));
	if (!b)
		return (NULL);
	sz = (size_t)tex->width * (size_t)tex->height
		* (size_t)tex->bytes_per_pixel;
	b->pixels = (unsigned char *)malloc(sz);
	if (!b->pixels)
		return (free(b), NULL);
	ft_memcpy(b->pixels, tex->pixels, sz);
	b->width = tex->width;
	b->height = tex->height;
	b->strength = strength;
	return (b);
}

/**
 * Assigns bump to object based on type
 */
static int	assign_bump_to_obj(t_obj *o, t_bump *b)
{
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
		return (1);
	return (0);
}

/**
 * Parses a bump token in the form "bm:<path>[:strength]"
 * Loads the PNG, copies pixels, and assigns bump to the given object.
 * Returns 0 on success, 1 on failure.
 */
int	parse_bump(const char *token, t_obj *o)
{
	char			*path;
	double			strength;
	mlx_texture_t	*tex;
	t_bump			*b;

	if (!token || ft_strncmp(token, "bm:", 3) != 0 || !o)
		return (1);
	if (parse_bump_token(token, &path, &strength))
		return (1);
	tex = mlx_load_png(path);
	free(path);
	if (!tex || !tex->pixels || !tex->width || !tex->height)
		return (ft_error("bump png load", 1));
	b = create_bump_from_tex(tex, strength);
	free(tex->pixels);
	free(tex);
	if (!b)
		return (ft_error("malloc", 1));
	if (assign_bump_to_obj(o, b))
	{
		free(b->pixels);
		free(b);
		return (1);
	}
	return (0);
}
