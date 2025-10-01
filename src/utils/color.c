/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 16:54:31 by apregitz          #+#    #+#             */
/*   Updated: 2025/09/30 21:17:18 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Converts RGB color struct to 32-bit integer format
 * Formula: (R << 24) | (G << 16) | (B << 8) | alpha
 * Used for MLX pixel rendering
 */
uint32_t	rgb_to_uint32(t_rgb *color)
{
	return ((uint32_t)((int)color->r << 24 | (int)color->g << 16 | (int)color->b << 8 | 0xf0));
}
