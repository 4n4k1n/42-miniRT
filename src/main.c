/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 11:00:01 by apregitz          #+#    #+#             */
/*   Updated: 2025/11/11 22:36:35 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static int	handle_master(int ac, char **av)
{
	uint32_t	port;

	if (ac < 3)
		return (printf("Error: Scene file required\n"), 1);
	port = get_port(ac, av);
	return (run_master(av[2], port));
}

static int	handle_worker(int ac, char **av)
{
	uint32_t	port;

	if (ac < 3)
		return (printf("Error: Master IP required\n"), 1);
	port = get_port(ac, av);
	return (run_worker(av[2], port));
}

void	translate_settings(t_data *data)
{
	data->defines.aa_max_samples = AA_MIN_SAMPLES * 8;
	data->defines.height = 1080 * (float)((float)SCALE / 100);
	data->defines.width = 1920 * (float)((float)SCALE / 100);
	data->defines.aspect_ratio = (double)data->defines.width
		/ (double)data->defines.height;
}

int	main(int ac, char **av)
{
	if (ac < 2)
	{
		print_usage(av[0]);
		return (1);
	}
	if (ft_strcmp(av[1], "--master") == 0)
		return (handle_master(ac, av));
	else if (ft_strcmp(av[1], "--worker") == 0)
		return (handle_worker(ac, av));
	return (run_local(av[1]));
}
