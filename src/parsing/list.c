/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 20:33:35 by nweber            #+#    #+#             */
/*   Updated: 2025/09/30 11:26:44 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Creates a new object node with specified type
 * @param type The type of object to create (SPHERE, PLANE, CYLINDER)
 * @return Pointer to new object or NULL if allocation fails
 */
t_obj	*obj_new(t_obj_type type)
{
	t_obj	*o;

	o = (t_obj *)malloc(sizeof(t_obj));
	if (!o)
		return (NULL);
	o->type = type;
	o->next = NULL;
	return (o);
}

/**
 * Adds an object node to the end of the object list
 * @param lst Pointer to object list
 * @param node Object node to add
 * @return 0 on success, 1 on failure
 */
int	obj_push(t_obj_list *lst, t_obj *node)
{
	if (!lst || !node)
		return (1);
	if (!lst->head)
		lst->head = node;
	else
		lst->tail->next = node;
	lst->tail = node;
	lst->size++;
	return (0);
}

/**
 * Creates a new light node
 * @return Pointer to new light or NULL if allocation fails
 */
t_light	*light_new(void)
{
	t_light	*l;

	l = (t_light *)malloc(sizeof(t_light));
	if (!l)
		return (NULL);
	l->next = NULL;
	return (l);
}

/**
 * Adds a light node to the end of the light list
 * @param lst Pointer to light list
 * @param node Light node to add
 * @return 0 on success, 1 on failure
 */
int	light_push(t_light_list *lst, t_light *node)
{
	if (!lst || !node)
		return (1);
	if (!lst->head)
		lst->head = node;
	else
		lst->tail->next = node;
	lst->tail = node;
	lst->size++;
	return (0);
}
