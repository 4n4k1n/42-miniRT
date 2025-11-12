/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monte_carlo_aa.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 01:06:04 by anakin            #+#    #+#             */
/*   Updated: 2025/11/12 12:49:08 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Normalizes accumulated samples and packs the final color.
 * Scales by 1/n, clamps to [0,255), writes aa->color, returns RGBA.
 * @param aa anti-aliasing accumulator (in/out)
 * @return packed RGBA color
 */
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

/**
 * Monte Carlo AA for a pixel with early exit.
 * Samples up to max_spp, updates variance, and stops if stable.
 * @param data global render state
 * @param i row index (y)
 * @param j column index (x)
 * @return packed RGBA color
 */
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
