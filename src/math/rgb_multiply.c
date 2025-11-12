/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rgb_multiply.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 10:57:00 by anakin            #+#    #+#             */
/*   Updated: 2025/11/12 16:25:58 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Scale an RGB color by a scalar factor.
 * Multiplies each channel of `color` by `t`. The result is not further
 * clamped here; callers should clamp before conversion to integer if needed.
 * @param color input color (0..255)
 * @param t scaling factor
 * @return scaled color (channels multiplied by t)
 */
__attribute__((always_inline)) inline t_rgb	rgb_multiply(const t_rgb color,
	const double t)
{
	return ((t_rgb){color.r * t, color.g * t, color.b * t});
}
