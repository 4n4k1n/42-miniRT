/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 10:28:41 by nweber            #+#    #+#             */
/*   Updated: 2025/07/07 12:50:04 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// adds the new node to the front
void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (lst && new)
	{
		new->next = *lst;
		*lst = new;
	}
}
