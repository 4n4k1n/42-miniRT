/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bounds.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 00:00:00 by anakin            #+#    #+#             */
/*   Updated: 2025/10/31 11:50:29 by nweber           ###   ########.fr       */
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
	t_vec3	n;
	t_vec3	tmp;
	t_vec3	t;
	t_vec3	b;
	double	hs;
	double	eps;
	t_vec3	c;
	t_vec3	p1;
	t_vec3	p2;
	t_vec3	p3;
	t_vec3	p4;
	t_vec3	expand;

	n = vec3_normalize(p->norm);
	tmp = (fabs(n.y) < 0.999) ? (t_vec3){0.0, 1.0, 0.0} : (t_vec3){1.0, 0.0, 0.0};
	t = vec3_normalize(vec3_cross(tmp, n));
	b = vec3_cross(n, t);
	hs = 10000.0;
	eps = 1.0;
	c = p->cords;
	p1 = vec3_add(c, vec3_add(vec3_multiply(t, hs), vec3_multiply(b, hs)));
	p2 = vec3_add(c, vec3_sub(vec3_multiply(t, hs), vec3_multiply(b, hs)));
	p3 = vec3_sub(c, vec3_sub(vec3_multiply(t, hs), vec3_multiply(b, hs)));
	p4 = vec3_sub(c, vec3_add(vec3_multiply(t, hs), vec3_multiply(b, hs)));
	box.min.x = fmin(fmin(p1.x, p2.x), fmin(p3.x, p4.x));
	box.min.y = fmin(fmin(p1.y, p2.y), fmin(p3.y, p4.y));
	box.min.z = fmin(fmin(p1.z, p2.z), fmin(p3.z, p4.z));
	box.max.x = fmax(fmax(p1.x, p2.x), fmax(p3.x, p4.x));
	box.max.y = fmax(fmax(p1.y, p2.y), fmax(p3.y, p4.y));
	box.max.z = fmax(fmax(p1.z, p2.z), fmax(p3.z, p4.z));
	expand = vec3_init(eps, eps, eps);
	box.min = vec3_sub(box.min, expand);
	box.max = vec3_add(box.max, expand);
	return (box);
}

static void	expand_cylinder_caps(t_aabb *box, t_vec3 center, \
		t_vec3 axis, double radius, double half_h)
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

// Conservative AABB for a finite cone: cover apex and base disk.
static t_aabb	get_cone_bounds(const t_cone *co)
{
	t_aabb	box;
	t_vec3	a;
	double	hh;
	double	r;
	t_vec3	apex;
	t_vec3	base;
	t_vec3	expand;
	t_vec3	eps;

	a = vec3_normalize(co->norm);
	hh = co->height * 0.5;
	r = co->diameter * 0.5;
	apex = vec3_sub(co->cords, vec3_multiply(a, hh));
	base = vec3_add(co->cords, vec3_multiply(a, hh));
	box.min.x = fmin(apex.x, base.x);
	box.min.y = fmin(apex.y, base.y);
	box.min.z = fmin(apex.z, base.z);
	box.max.x = fmax(apex.x, base.x);
	box.max.y = fmax(apex.y, base.y);
	box.max.z = fmax(apex.z, base.z);
	expand = vec3_init(r, r, r);
	box.min = vec3_sub(box.min, expand);
	box.max = vec3_add(box.max, expand);
	eps = vec3_init(1e-6, 1e-6, 1e-6);
	box.min = vec3_sub(box.min, eps);
	box.max = vec3_add(box.max, eps);
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
	else if (obj->type == CONE)
		return (get_cone_bounds(&obj->data.cone));
	return ((t_aabb){{0, 0, 0}, {0, 0, 0}});
}
