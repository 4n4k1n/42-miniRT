/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 10:30:25 by nweber            #+#    #+#             */
/*   Updated: 2025/07/07 12:55:43 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// counts the nodes in the list
int	ft_lstsize(t_list *lst)
{
	int	count;

	count = 0;
	while (lst)
	{
		count++;
		lst = lst->next;
	}
	return (count);
}
