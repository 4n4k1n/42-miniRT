/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation_mat.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 13:35:08 by nweber            #+#    #+#             */
/*   Updated: 2025/11/06 13:35:10 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static int	mat_base_for_obj(t_obj *o, int *base_len, t_rgb *albedo,
			t_material ***outp)
{
	if (!o)
		return (1);
	if (o->type == SPHERE || o->type == PLANE)
	{
		*base_len = 4;
		if (o->type == SPHERE)
		{
			*albedo = o->data.sphere.rgb;
			*outp = &o->data.sphere.mat;
		}
		else
		{
			*albedo = o->data.plane.rgb;
			*outp = &o->data.plane.mat;
		}
		return (0);
	}
	if (o->type == CYLINDER || o->type == PYRAMID || o->type == CONE)
	{
		*base_len = 6;
		if (o->type == CYLINDER)
		{
			*albedo = o->data.cylinder.rgb;
			*outp = &o->data.cylinder.mat;
		}
		else if (o->type == PYRAMID)
		{
			*albedo = o->data.pyramid.rgb;
			*outp = &o->data.pyramid.mat;
		}
		else
		{
			*albedo = o->data.cone.rgb;
			*outp = &o->data.cone.mat;
		}
		return (0);
	}
	if (o->type == TRIANGLE)
	{
		*base_len = 5;
		*albedo = o->data.triangle.rgb;
		*outp = &o->data.triangle.mat;
		return (0);
	}
	return (1);
}

static int	create_material_from_token(const char *mstr, t_rgb albedo,
			t_material **out)
{
	double		ri;
	const char	*s;

	if (mstr[0] == 'L')
	{
		*out = material_lambertian(albedo);
		return (*out == NULL);
	}
	if (mstr[0] == 'M')
	{
		*out = material_metal(albedo, 0.0);
		return (*out == NULL);
	}
	if (mstr[0] == 'G')
	{
		ri = 1.5;
		s = ft_strchr(mstr, ':');
		if (s)
		{
			s++;
			if (parse_double(s, &ri))
				return (1);
			if (ri <= 0.0)
				ri = 1.5;
		}
		*out = material_dielectric(ri);
		return (*out == NULL);
	}
	return (1);
}

static int	parse_texture_token(const char *tok, t_material **out)
{
	const char	*rest;
	double		scale;

	rest = tok + 3;
	scale = 1.0;
	if (ft_strncmp(rest, "checker", 7) != 0)
		return (1);
	rest += 7;
	if (*rest == ':')
	{
		rest++;
		if (parse_double(rest, &scale))
			return (1);
		if (scale <= 0.0)
			scale = 1.0;
	}
	(*out)->texture_type = CHECKER;
	(*out)->texture_scale = scale;
	(*out)->texture_a = (t_rgb){255.0, 255.0, 255.0};
	(*out)->texture_b = (t_rgb){0.0, 0.0, 0.0};
	return (0);
}

int	parse_material(char **tokens, int len, t_obj *o)
{
	int			base_len;
	int			i;
	const char	*mstr;
	t_rgb		albedo;
	t_material	**out;
	const char	*rest;
	double		scale;

	if (mat_base_for_obj(o, &base_len, &albedo, &out))
		return (1);
	mstr = "L";
	i = base_len;
	if (len > base_len && (tokens[i][0] == 'L'
		|| tokens[i][0] == 'M' || tokens[i][0] == 'G'))
	{
		mstr = tokens[i];
		i++;
	}
	if (create_material_from_token(mstr, albedo, out))
		return (1);
	while (i < len)
	{
		if (ft_strncmp(tokens[i], "tx:", 3) == 0)
		{
			rest = tokens[i] + 3;
			scale = 1.0;
			if (ft_strncmp(rest, "checker", 7) != 0)
				return (1);
			rest += 7;
			if (*rest == ':')
			{
				rest++;
				if (parse_double(rest, &scale))
					return (1);
				if (scale <= 0.0)
					scale = 1.0;
			}
			(*out)->texture_type = CHECKER;
			(*out)->texture_scale = scale;
			(*out)->texture_a = (t_rgb){255.0, 255.0, 255.0};
			(*out)->texture_b = (t_rgb){0.0, 0.0, 0.0};
		}
		i++;
	}
	return (0);
}
