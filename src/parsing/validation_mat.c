/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation_mat.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 13:35:08 by nweber            #+#    #+#             */
/*   Updated: 2025/11/07 12:15:18 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static int	init_mat_ctx(t_obj *o, t_mat_ctx *ctx)
{
	if (mat_base_for_obj(o, &ctx->base_len, &ctx->albedo, &ctx->out))
		return (1);
	ctx->i = ctx->base_len;
	return (0);
}

static int	apply_mat_token(char **tokens, int len, t_mat_ctx *ctx)
{
	const char	*mstr;

	mstr = NULL;
	if (ctx->i < len && (tokens[ctx->i][0] == 'L'
		|| tokens[ctx->i][0] == 'M' || tokens[ctx->i][0] == 'G'
		|| tokens[ctx->i][0] == 'P'))
	{
		mstr = tokens[ctx->i];
		ctx->i++;
	}
	if (mstr && create_material_from_token(mstr, ctx->albedo, ctx->out))
		return (1);
	return (0);
}

static int	apply_texture_token(const char *rest, t_material **out)
{
	double	scale;

	if (ft_strncmp(rest, "checker", 7) != 0)
		return (1);
	scale = 1.0;
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

static int	parse_material_extras(char **tokens, int len, t_mat_ctx *ctx)
{
	while (ctx->i < len)
	{
		if (ft_strncmp(tokens[ctx->i], "tx:", 3) == 0)
		{
			if (!*ctx->out)
				return (1);
			if (apply_texture_token(tokens[ctx->i] + 3, ctx->out))
				return (1);
		}
		ctx->i++;
	}
	return (0);
}

int	parse_material(char **tokens, int len, t_obj *o)
{
	t_mat_ctx	ctx;

	if (init_mat_ctx(o, &ctx))
		return (1);
	if (apply_mat_token(tokens, len, &ctx))
		return (1);
	if (parse_material_extras(tokens, len, &ctx))
		return (1);
	return (0);
}
