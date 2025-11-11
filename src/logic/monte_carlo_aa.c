/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monte_carlo_aa.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 01:06:04 by anakin            #+#    #+#             */
/*   Updated: 2025/11/07 13:27:00 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

uint32_t	finalize_color(t_anti_aliasing *aa)
{
	aa->inv_n = 1.0 / aa->n;
	aa->rbyte = (256.0 * clamp((aa->acc_r * aa->inv_n) / 255.999, 0.0, 0.999));
	aa->gbyte = (256.0 * clamp((aa->acc_g * aa->inv_n) / 255.999, 0.0, 0.999));
	aa->bbyte = (256.0 * clamp((aa->acc_b * aa->inv_n) / 255.999, 0.0, 0.999));
	aa->color.r = aa->rbyte;
	aa->color.g = aa->gbyte;
	aa->color.b = aa->bbyte;
	return (rgb_to_uint32(&aa->color));
}

uint32_t	monte_carlo_aa(t_data *data, int i, int j)
{
	t_anti_aliasing	aa;

	aa = data->aa;
	init_aa(&aa);
	while (aa.n < aa.max_spp)
	{
		sample_pixel(data, &aa, i, j);
		update_variance(&aa);
		if (!should_continue(&aa))
			break ;
	}
	return (finalize_color(&aa));
}
