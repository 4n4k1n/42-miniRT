/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 13:51:00 by nweber            #+#    #+#             */
/*   Updated: 2025/11/06 13:51:00 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static void	print_ambient(const t_ambient *a)
{
	if (!a)
		return ;
	printf("Ambient:\n");
	printf("  ratio: %.6f\n", a->lighting);
	printf("  ");
	print_rgb("color=", a->color);
	printf("\n");
}

static void	print_camera(const t_camera *c)
{
	if (!c)
		return ;
	printf("Camera:\n");
	printf("  ");
	print_vec3("pos=", c->cords);
	printf("\n  ");
	print_vec3("dir=", c->orientation);
	printf("\n");
	printf("  fov: %.6f\n", c->foc);
}

static void	print_lights(const t_light_list *lst)
{
	const t_light	*cur;
	size_t			i;
	size_t			count;

	if (lst)
		count = lst->size;
	else
		count = 0;
	printf("Lights (count=%zu):\n", count);
	if (!lst)
		return ;
	cur = lst->head;
	i = 0;
	while (cur)
	{
		printf("  [%zu] ", i);
		print_vec3("pos=", cur->cords);
		printf(" ");
		print_rgb("color=", cur->color);
		printf(" intensity=%.2f", cur->intensity);
		printf("\n");
		cur = cur->next;
		i++;
	}
}

void	print_scene(const t_data *scene)
{
	if (!scene)
		return ;
	printf("==== Parsed Scene ====\n");
	print_ambient(&scene->ambiente);
	print_camera(&scene->camera);
	print_lights(scene->light_list);
	print_objects(scene->objects);
	printf("======================\n");
}
