/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 18:26:42 by nweber            #+#    #+#             */
/*   Updated: 2025/09/29 20:26:26 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	is_rt_file(const char *path)
{
	size_t	len;

	if (!path)
		return (0);
	len = ft_strlen(path);
	if (len < 3)
		return (0);
	if (len >= 3 && path[len - 3] == '.' && path[len - 2] == 'r'
		&& path[len - 1] == 't')
		return (1);
	return (0);
}

int	parse_double(const char *str, double *out)
{
	if (!str || !out)
		return (0);
	*out = ft_atof(str);
	return (1);
}

int	parse_vec3(const char *str, t_vec3 *out)
{
	char	**tokens;

	if (!str || !out)
		return (0);
	tokens = ft_split(str, ',');
	if (!tokens)
		return (0);
	if (ft_arrlen((void **)tokens) != 3)
		return (ft_array_free(tokens), 1);
	out->x = ft_atof(tokens[0]);
	out->y = ft_atof(tokens[1]);
	out->z = ft_atof(tokens[2]);
	ft_array_free(tokens);
	return (1);
}

int	parse_rgb(const char *str, t_rgb *out)
{
	char	**tokens;
	int		r;
	int		g;
	int		b;

	if (!str || !out)
		return (0);
	tokens = ft_split(str, ',');
	if (!tokens)
		return (0);
	if (ft_arrlen((void **)tokens) != 3)
		return (ft_array_free(tokens), 1);
	r = (int)ft_atof(tokens[0]);
	g = (int)ft_atof(tokens[1]);
	b = (int)ft_atof(tokens[2]);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		return (ft_array_free(tokens), 1);
	out->r = r;
	out->g = g;
	out->b = b;
	ft_array_free(tokens);
	return (0);
}
