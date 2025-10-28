/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_cleanup.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 00:00:00 by anakin            #+#    #+#             */
/*   Updated: 2025/10/28 00:00:00 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Recursively frees BVH tree nodes
 */
void	free_bvh(t_bvh_node *node)
{
	if (!node)
		return ;
	if (node->left)
		free_bvh(node->left);
	if (node->right)
		free_bvh(node->right);
	if (node->objects)
		free(node->objects);
	free(node);
}
