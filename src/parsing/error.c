/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apregitz <apregitz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 18:52:57 by nweber            #+#    #+#             */
/*   Updated: 2025/10/10 16:57:38 by apregitz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Prints error message to stderr with "Error\n" prefix
 * @param msg The error message to display
 * @return Always returns 1 for error propagation
 */
int	rt_error(const char *msg)
{
	if (msg)
	{
		if (write(2, "Error\n", 6) == -1 || write(2, msg, ft_strlen(msg)) == -1 || write(2, "\n", 1) == -1)
			return (1);
	}
	return (1);
}

/**
 * Frees all objects in the object list and the list itself
 * @param lst Pointer to object list to free
 */
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
		if (cur->type == SPHERE && cur->data.sphere.mat)
			free(cur->data.sphere.mat);
		if (cur->type == PLANE && cur->data.plane.mat)
			free(cur->data.plane.mat);
		if (cur->type == CYLINDER && cur->data.cylinder.mat)
			free(cur->data.cylinder.mat);
		free(cur);
		cur = nx;
	}
	free(lst);
}

/**
 * Frees all lights in the light list and the list itself
 * @param lst Pointer to light list to free
 */
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

/**
 * Frees all allocated memory in the scene data structure
 * @param scene Pointer to scene data structure to free
 */
void	free_scene(t_data *scene)
{
	if (!scene)
		return ;
	free_objects(scene->objects);
	free_lights(scene->light_list);
	scene->objects = NULL;
	scene->light_list = NULL;
}
