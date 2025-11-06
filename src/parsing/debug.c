/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 13:34:56 by nweber            #+#    #+#             */
/*   Updated: 2025/11/06 13:35:00 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

void	print_vec3(const char *name, t_vec3 v)
{
	printf("%s(%.6f, %.6f, %.6f)", name, v.x, v.y, v.z);
}

void	print_rgb(const char *name, t_rgb c)
{
	printf("%s(%d, %d, %d)", name, (int)c.r, (int)c.g, (int)c.b);
}

static void	print_material(t_material *mat)
{
	if (!mat)
	{
		printf(" mat=NULL ");
		return ;
	}
	if (mat->type == LAMBERTIAN)
		printf(" mat=LAMBERTIAN ");
	if (mat->type == METAL)
		printf(" mat=METAL ");
}

static void	print_object_one(const t_obj *o, size_t idx)
{
	if (o->type == SPHERE)
	{
		printf("  [%zu] sp: ", idx);
		print_vec3("center=", o->data.sphere.cords);
		printf(" dia=%.6f ", o->data.sphere.diameter);
		print_rgb("rgb=", o->data.sphere.rgb);
		print_material(o->data.sphere.mat);
		printf("\n");
	}
	else if (o->type == PLANE)
	{
		printf("  [%zu] pl: ", idx);
		print_vec3("point=", o->data.plane.cords);
		printf(" ");
		print_vec3("norm=", o->data.plane.norm);
		printf(" ");
		print_rgb("rgb=", o->data.plane.rgb);
		printf("\n");
	}
	else if (o->type == CYLINDER)
	{
		printf("  [%zu] cy: ", idx);
		print_vec3("center=", o->data.cylinder.cords);
		printf(" ");
		print_vec3("axis=", o->data.cylinder.norm);
		printf(" dia=%.6f h=%.6f ", o->data.cylinder.diameter,
			o->data.cylinder.height);
		print_rgb("rgb=", o->data.cylinder.rgb);
		printf("\n");
	}
	else if (o->type == PYRAMID)
	{
		printf("  [%zu] py: ", idx);
		print_vec3("center=", o->data.pyramid.cords);
		printf(" ");
		print_vec3("axis=", o->data.pyramid.norm);
		printf(" base=%.6f h=%.6f ", o->data.pyramid.base,
			o->data.pyramid.height);
		print_rgb("rgb=", o->data.pyramid.rgb);
		printf("\n");
	}
}

void	print_objects(const t_obj_list *lst)
{
	const t_obj	*cur;
	size_t		i;
	size_t		count;

	if (lst)
		count = lst->size;
	else
		count = 0;
	printf("Objects (count=%zu):\n", count);
	if (!lst)
		return ;
	cur = lst->head;
	i = 0;
	while (cur)
	{
		print_object_one(cur, i);
		cur = cur->next;
		i++;
	}
}
