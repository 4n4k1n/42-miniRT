/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worker_tile.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 13:00:00 by anakin            #+#    #+#             */
/*   Updated: 2025/11/12 16:50:26 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Copy a single packed 32-bit pixel into an MLX image buffer.
 * The MLX image stores pixels as 4 consecutive bytes per pixel (RGBA).
 * This helper extracts components from the packed pixel and writes them
 * into the image byte array at the specified index.
 * @param image destination MLX image
 * @param pixel packed 0xRRGGBBAA pixel value
 * @param img_idx linear pixel index inside the image
 */
static void	copy_pixel_to_image(mlx_image_t *image, uint32_t pixel,
	uint32_t img_idx)
{
	image->pixels[img_idx * 4 + 0] = (pixel >> 24) & 0xFF;
	image->pixels[img_idx * 4 + 1] = (pixel >> 16) & 0xFF;
	image->pixels[img_idx * 4 + 2] = (pixel >> 8) & 0xFF;
	image->pixels[img_idx * 4 + 3] = pixel & 0xFF;
}

/**
 * Copy a single row from a tile result into the global framebuffer image.
 * Translates tile-local coordinates to image coordinates and uses
 * copy_pixel_to_image for each pixel in the row.
 * @param image destination MLX image
 * @param result metadata describing tile position and size
 * @param pixels tile pixel buffer
 * @param y row index within the tile (0..height-1)
 */
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

/**
 * Copy an entire tile's pixels into the framebuffer image.
 * Iterates rows of the tile and copies them into the MLX image at the
 * tile's offset.
 * @param image destination MLX image
 * @param result tile metadata and placement
 * @param pixels pixel buffer for the tile
 */
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
