/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 20:33:35 by nweber            #+#    #+#             */
/*   Updated: 2025/09/29 20:35:45 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

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

t_light	*light_new(void)
{
	t_light	*l;

	l = (t_light *)malloc(sizeof(t_light));
	if (!l)
		return (NULL);
	l->next = NULL;
	return (l);
}

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
