/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rgb_modulate.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 10:56:46 by anakin            #+#    #+#             */
/*   Updated: 2025/11/12 16:25:45 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Component-wise modulation (Hadamard product) of two colors.
 * Treats input colors in the 0..255 range and returns a color where each
 * component is (a.c * b.c) / 255.999 to map the result back into 0..255.
 * The small epsilon prevents exact 255 -> 256 rounding during later casts.
 * @param a left color
 * @param b right color
 * @return modulated color
 */
__attribute__((always_inline)) inline t_rgb	rgb_modulate(const t_rgb a,
	const t_rgb b)
{
	return ((t_rgb){(a.r * b.r) / 255.999, (a.g * b.g) / 255.999, (a.b * b.b)
		/ 255.999});
}
