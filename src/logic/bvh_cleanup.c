/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_cleanup.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 00:00:00 by anakin            #+#    #+#             */
/*   Updated: 2025/11/12 13:41:39 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Recursively frees a BVH subtree.
 * Frees child subtrees first, then the node's object array (if present),
 * and finally the node itself. Safe to call with NULL.
 * @param node root of the BVH subtree to free
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
