/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mat_token_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 15:20:00 by anakin            #+#    #+#             */
/*   Updated: 2025/11/12 12:29:16 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Creates Phong material from token.
 * Format: P[:shininess]
 * Default shininess 32.0 (reset if < 1.0).
 * @param mstr token (e.g. "P:64")
 * @param albedo base color
 * @param out receives allocated material
 * @return 0 success, 1 failure
 */
int	create_phong_material(const char *mstr, t_rgb albedo, t_material **out)
{
	double		ri;
	const char	*s;

	ri = 32.0;
	s = ft_strchr(mstr, ':');
	if (s)
	{
		s++;
		if (parse_double(s, &ri))
			return (1);
		if (ri < 1.0)
			ri = 32.0;
	}
	*out = material_phong(albedo, ri);
	return (*out == NULL);
}

/**
 * Creates Glass (dielectric) material from token.
 * Format: G[:ior]
 * Default ior 1.5 (reset if <= 0.0).
 * @param mstr token (e.g. "G:1.3")
 * @param out receives allocated material
 * @return 0 success, 1 failure
 */
int	create_glass_material(const char *mstr, t_material **out)
{
	double		ri;
	const char	*s;

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
