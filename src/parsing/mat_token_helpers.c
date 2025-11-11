/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mat_token_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 15:20:00 by anakin            #+#    #+#             */
/*   Updated: 2025/11/11 15:20:00 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

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
