/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bounds.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 00:00:00 by anakin            #+#    #+#             */
/*   Updated: 2025/10/30 14:31:33 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Calculates bounding box for triangle
 * Creates box containing all three vertices
 */
t_aabb	get_triangle_bounds(const t_triangle *tri)
{
	t_aabb	box;

	box.min.x = fmin(fmin(tri->v0.x, tri->v1.x), tri->v2.x);
	box.min.y = fmin(fmin(tri->v0.y, tri->v1.y), tri->v2.y);
	box.min.z = fmin(fmin(tri->v0.z, tri->v1.z), tri->v2.z);
	box.max.x = fmax(fmax(tri->v0.x, tri->v1.x), tri->v2.x);
	box.max.y = fmax(fmax(tri->v0.y, tri->v1.y), tri->v2.y);
	box.max.z = fmax(fmax(tri->v0.z, tri->v1.z), tri->v2.z);
	return (box);
}

/**
 * Calculates axis-aligned bounding box for sphere
 */
t_aabb	get_sphere_bounds(const t_sphere *s)
{
	t_aabb	box;
	double	radius;

	radius = s->diameter * 0.5;
	box.min.x = s->cords.x - radius;
	box.min.y = s->cords.y - radius;
	box.min.z = s->cords.z - radius;
	box.max.x = s->cords.x + radius;
	box.max.y = s->cords.y + radius;
	box.max.z = s->cords.z + radius;
	return (box);
}

/**
 * Calculates bounding box for plane
 * Uses large but finite box to avoid infinite bounds
 * Reduced from 1e6 to 100 to improve BVH partitioning
 */
t_aabb	get_plane_bounds(const t_plane *p)
{
	t_aabb	box;

	(void)p;
	box.min.x = -100.0;
	box.min.y = -100.0;
	box.min.z = -100.0;
	box.max.x = 100.0;
	box.max.y = 100.0;
	box.max.z = 100.0;
	return (box);
}

static void	expand_cylinder_caps(t_aabb *box, t_vec3 center, t_vec3 axis,
		double radius, double half_h)
{
	t_vec3	top;
	t_vec3	bottom;
	t_vec3	offset;

	offset = vec3_multiply(axis, half_h);
	top = vec3_add(center, offset);
	bottom = vec3_sub(center, offset);
	*box = aabb_expand_point(*box, vec3_add(top, vec3_init(radius, 0, 0)));
	*box = aabb_expand_point(*box, vec3_sub(top, vec3_init(radius, 0, 0)));
	*box = aabb_expand_point(*box, vec3_add(top, vec3_init(0, radius, 0)));
	*box = aabb_expand_point(*box, vec3_sub(top, vec3_init(0, radius, 0)));
	*box = aabb_expand_point(*box, vec3_add(top, vec3_init(0, 0, radius)));
	*box = aabb_expand_point(*box, vec3_sub(top, vec3_init(0, 0, radius)));
	*box = aabb_expand_point(*box, vec3_add(bottom, vec3_init(radius, 0, 0)));
	*box = aabb_expand_point(*box, vec3_sub(bottom, vec3_init(radius, 0, 0)));
	*box = aabb_expand_point(*box, vec3_add(bottom, vec3_init(0, radius, 0)));
	*box = aabb_expand_point(*box, vec3_sub(bottom, vec3_init(0, radius, 0)));
	*box = aabb_expand_point(*box, vec3_add(bottom, vec3_init(0, 0, radius)));
	*box = aabb_expand_point(*box, vec3_sub(bottom, vec3_init(0, 0, radius)));
}

/**
 * Calculates axis-aligned bounding box for cylinder
 */
t_aabb	get_cylinder_bounds(const t_cylinder *c)
{
	t_aabb	box;
	double	radius;
	double	half_h;
	t_vec3	axis;

	radius = c->diameter * 0.5;
	half_h = c->height * 0.5;
	axis = vec3_normalize(c->norm);
	box.min = vec3_init(INFINITY, INFINITY, INFINITY);
	box.max = vec3_init(-INFINITY, -INFINITY, -INFINITY);
	expand_cylinder_caps(&box, c->cords, axis, radius, half_h);
	return (box);
}

/**
 * Gets bounding box for any object type
 */
t_aabb	get_object_bounds(const t_obj *obj)
{
	if (obj->type == SPHERE)
		return (get_sphere_bounds(&obj->data.sphere));
	else if (obj->type == PLANE)
		return (get_plane_bounds(&obj->data.plane));
	else if (obj->type == CYLINDER)
		return (get_cylinder_bounds(&obj->data.cylinder));
	else if (obj->type == TRIANGLE)
		return (get_triangle_bounds(&obj->data.triangle));
	return ((t_aabb){{0, 0, 0}, {0, 0, 0}});
}
