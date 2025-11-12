/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 17:30:00 by anakin            #+#    #+#             */
/*   Updated: 2025/11/12 15:41:21 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Returns the current time in milliseconds since the Epoch.
 * Uses gettimeofday and converts seconds + microseconds to milliseconds.
 * @return current time in ms (int)
 */
int	get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

/**
 * Frees all objects in the provided object list and the list container.
 * Frees per-object material pointers when present and then the object nodes.
 * Safe to call with NULL.
 * @param objects pointer to object list to clean up
 */
void	cleanup_objects(t_obj_list *objects)
{
	t_obj	*current;
	t_obj	*next;

	if (!objects)
		return ;
	current = objects->head;
	while (current)
	{
		next = current->next;
		if (current->type == SPHERE && current->data.sphere.mat)
			free(current->data.sphere.mat);
		if (current->type == PLANE && current->data.plane.mat)
			free(current->data.plane.mat);
		if (current->type == CYLINDER && current->data.cylinder.mat)
			free(current->data.cylinder.mat);
		if (current->type == TRIANGLE && current->data.triangle.mat)
			free(current->data.triangle.mat);
		free(current);
		current = next;
	}
	free(objects);
}

/**
 * Frees all lights in the provided light list and the list container.
 * Safe to call with NULL.
 * @param lights pointer to light list to clean up
 */
void	cleanup_lights(t_light_list *lights)
{
	t_light	*current;
	t_light	*next;

	if (!lights)
		return ;
	current = lights->head;
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}
	free(lights);
}

/**
 * Prints program usage/help to stdout.
 * @param prog program name (argv[0]) used in usage lines
 */
void	print_usage(char *prog)
{
	printf("Usage:\n");
	printf("  Local:  %s scene.rt\n", prog);
	printf("  Master: %s --master scene.rt [--port 9000]\n", prog);
	printf("  Worker: %s --worker <master_ip> [--port 9000]\n", prog);
}

/**
 * Parses an optional --port argument and returns the chosen port.
 * If --port <n> is present at argv[3]/argv[4], returns that value,
 * otherwise returns the default port 9000.
 * @param ac argument count
 * @param av argument vector
 * @return selected port number
 */
uint32_t	get_port(int ac, char **av)
{
	if (ac >= 5 && ft_strcmp(av[3], "--port") == 0)
		return (ft_atoi(av[4]));
	return (9000);
}
