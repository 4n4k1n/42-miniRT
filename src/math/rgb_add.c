/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rgb_add.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 10:56:32 by anakin            #+#    #+#             */
/*   Updated: 2025/11/12 16:25:34 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Component-wise addition of two RGB colors with clamping.
 * Adds corresponding components of `a` and `b` and clamps each channel to
 * a safe upper bound slightly below 256 to avoid overflow when later
 * converting to integers. Returns the resulting t_rgb.
 * @param a first color
 * @param b second color
 * @return clamped component-wise sum of a and b
 */
__attribute__((always_inline)) inline t_rgb	rgb_add(const t_rgb a,
	const t_rgb b)
{
	return ((t_rgb){fmin(a.r + b.r, 255.999), fmin(a.g + b.g, 255.999), fmin(a.b
			+ b.b, 255.999)});
}
