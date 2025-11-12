/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 11:00:01 by apregitz          #+#    #+#             */
/*   Updated: 2025/11/12 15:39:30 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Handle running as the master node.
 * Expects: program --master <scene-file> [port]
 * Validates arguments, extracts optional port and starts master server.
 * @param ac argument count
 * @param av argument vector
 * @return exit code from run_master (0 on success)
 */
static int	handle_master(int ac, char **av)
{
	uint32_t	port;

	if (ac < 3)
	{
		printf("Error: Scene file required\n");
		return (1);
	}
	port = get_port(ac, av);
	return (run_master(av[2], port));
}

/**
 * Handle running as a worker node.
 * Expects: program --worker <master-ip> [port]
 * Validates arguments, extracts optional port and starts worker client.
 * @param ac argument count
 * @param av argument vector
 * @return exit code from run_worker (0 on success)
 */
static int	handle_worker(int ac, char **av)
{
	uint32_t	port;

	if (ac < 3)
	{
		printf("Error: Master IP required\n");
		return (1);
	}
	port = get_port(ac, av);
	return (run_worker(av[2], port));
}

/**
 * Convert compile-time / parsed settings into runtime rendering defines.
 * Sets:
 *  - aa_max_samples: maximum anti-aliasing samples allowed
 *  - width / height: image resolution derived from SCALE and aspect ratio
 *  - aspect_ratio: width / height
 * @param data global scene / runtime data to populate
 */
void	translate_settings(t_data *data)
{
	data->defines.aa_max_samples = AA_MIN_SAMPLES * 8;
	data->defines.height = 1080 * (float)((float)SCALE / 100);
	data->defines.width = 1920 * (float)((float)SCALE / 100);
	data->defines.aspect_ratio = (double)data->defines.width
		/ (double)data->defines.height;
}

/**
 * Program entrypoint.
 * Usage:
 *  - program <scene.rt>           => run locally
 *  - program --master <scene>     => run as master (optional port)
 *  - program --worker <masterip>  => run as worker (optional port)
 * Dispatches to the appropriate run_* helper based on argv[1].
 * @param ac arg count
 * @param av arg vector
 * @return process exit code
 */
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
