/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 18:52:57 by nweber            #+#    #+#             */
/*   Updated: 2025/09/29 20:32:14 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	rt_error(const char *msg)
{
	if (msg)
	{
		write(2, "Error\n", 6);
		write(2, msg, ft_strlen(msg));
		write(2, "\n", 1);
	}
	return (1);
}

static void	free_objects(t_obj_list *lst)
{
	t_obj	*cur;
	t_obj	*nx;

	if (!lst)
		return ;
	cur = lst->head;
	while (cur)
	{
		nx = cur->next;
		free(cur);
		cur = nx;
	}
	free(lst);
}

static void	free_lights(t_light_list *lst)
{
	t_light	*cur;
	t_light	*nx;

	if (!lst)
		return ;
	cur = lst->head;
	while (cur)
	{
		nx = cur->next;
		free(cur);
		cur = nx;
	}
	free(lst);
}

void	free_scene(t_main *scene)
{
	if (!scene)
		return ;
	free_objects(scene->objects);
	free_lights(scene->light_list);
	scene->objects = NULL;
	scene->light_list = NULL;
}
