/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bounds_simple.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 00:00:00 by anakin            #+#    #+#             */
/*   Updated: 2025/10/28 00:00:00 by anakin           ###   ########.fr       */
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

static void	expand_cap_points(t_aabb *box, t_vec3 cap, double radius)
{
	*box = aabb_expand_point(*box, vec3_add(cap, vec3_init(radius, 0, 0)));
	*box = aabb_expand_point(*box, vec3_sub(cap, vec3_init(radius, 0, 0)));
	*box = aabb_expand_point(*box, vec3_add(cap, vec3_init(0, radius, 0)));
	*box = aabb_expand_point(*box, vec3_sub(cap, vec3_init(0, radius, 0)));
	*box = aabb_expand_point(*box, vec3_add(cap, vec3_init(0, 0, radius)));
	*box = aabb_expand_point(*box, vec3_sub(cap, vec3_init(0, 0, radius)));
}

static void	expand_cylinder_caps(t_aabb *box, t_cyl_bounds *cb)
{
	t_vec3	top;
	t_vec3	bottom;
	t_vec3	offset;

	offset = vec3_multiply(cb->axis, cb->half_h);
	top = vec3_add(cb->center, offset);
	bottom = vec3_sub(cb->center, offset);
	expand_cap_points(box, top, cb->radius);
	expand_cap_points(box, bottom, cb->radius);
}

/**
 * Calculates axis-aligned bounding box for cylinder
 */
t_aabb	get_cylinder_bounds(const t_cylinder *c)
{
	t_aabb			box;
	t_cyl_bounds	cb;

	cb.radius = c->diameter * 0.5;
	cb.half_h = c->height * 0.5;
	cb.axis = vec3_normalize(c->norm);
	cb.center = c->cords;
	box.min = vec3_init(INFINITY, INFINITY, INFINITY);
	box.max = vec3_init(-INFINITY, -INFINITY, -INFINITY);
	expand_cylinder_caps(&box, &cb);
	return (box);
}
