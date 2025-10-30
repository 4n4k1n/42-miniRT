/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worker_tile.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 13:00:00 by anakin            #+#    #+#             */
/*   Updated: 2025/10/30 13:00:00 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static void	copy_pixel_to_image(mlx_image_t *image, uint32_t pixel,
	uint32_t img_idx)
{
	image->pixels[img_idx * 4 + 0] = (pixel >> 24) & 0xFF;
	image->pixels[img_idx * 4 + 1] = (pixel >> 16) & 0xFF;
	image->pixels[img_idx * 4 + 2] = (pixel >> 8) & 0xFF;
	image->pixels[img_idx * 4 + 3] = pixel & 0xFF;
}

static void	copy_tile_row(mlx_image_t *image, t_tile *result,
	uint32_t *pixels, uint32_t y)
{
	uint32_t	x;
	uint32_t	img_idx;
	uint32_t	pixel_idx;

	x = 0;
	while (x < result->width)
	{
		pixel_idx = y * result->width + x;
		img_idx = (result->y + y) * image->width + (result->x + x);
		copy_pixel_to_image(image, pixels[pixel_idx], img_idx);
		x++;
	}
}

void	copy_tile_to_framebuffer(mlx_image_t *image, t_tile *result,
		uint32_t *pixels)
{
	uint32_t	y;

	y = 0;
	while (y < result->height)
	{
		copy_tile_row(image, result, pixels, y);
		y++;
	}
}
