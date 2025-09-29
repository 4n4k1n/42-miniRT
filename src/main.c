/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apregitz <apregitz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 11:00:01 by apregitz          #+#    #+#             */
/*   Updated: 2025/09/29 19:27:35 by apregitz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

int	main(int ac, char **av)
{
	t_data	data;
	int	i;
	int	j;
	
	(void)ac;
	(void)av;

	mlx_set_setting(MLX_MAXIMIZED, true);
	mlx_t* mlx = mlx_init(WIDTH, HEIGHT, "42Balls", false);
	if (!mlx)
		return (1);
	mlx_image_t* img = mlx_new_image(mlx, 256, 256);
	if (!img || (mlx_image_to_window(mlx, img, 0, 0) < 0))
		return (1);

	i = 0;
	while (i < data.height)
	{
		j = 0;
		while (j < data.width)
		{
			
		}
	}

	mlx_loop(mlx);
	mlx_terminate(mlx);

	return (0);
}
