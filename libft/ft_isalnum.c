/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 16:38:57 by nweber            #+#    #+#             */
/*   Updated: 2025/07/07 12:41:13 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// checks if the input is a digit or alphabetic
int	ft_isalnum(int c)
{
	return (ft_isalpha(c) || ft_isdigit(c));
}
