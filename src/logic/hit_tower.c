#include "mini_rt.h"

/**
 * Normalizes the cylinder axis vector
 * Stores normalized axis and its original length
 * Returns 0 if axis vector length is zero, else 1
 */
static int	cyl_valid_axis(const t_cylinder *cyl, t_cyl_hit *ch)
{
	ch->axis = cyl->norm;
	ch->axis_len = vec3_sqrt(&ch->axis);
	if (ch->axis_len == 0.0)
		return (0);
	ch->axis = vec3_divide_inline(&ch->axis, ch->axis_len);
	return (1);
}

/**
 * Selects the valid quadratic root within range
 * Evaluates both roots against t interval and height limits
 * Computes intersection point when valid root is found
 * Returns 1 on success, 0 on failure
 */
static int	cyl_select_root(t_cyl_hit *ch, t_ray *r, double tmin, double tmax)
{
	double	sq;

	sq = sqrt(ch->disc);
	ch->root = (-ch->h - sq) / ch->a;
	if (ch->root <= tmin || ch->root >= tmax)
		ch->root = (-ch->h + sq) / ch->a;
	if (ch->root <= tmin || ch->root >= tmax)
		return (0);
	ch->s = ch->k_dot_a + ch->root * ch->d_dot_a;
	if (fabs(ch->s) > ch->half_h)
	{
		ch->root = (-ch->h + sq) / ch->a;
		if (ch->root <= tmin || ch->root >= tmax)
			return (0);
		ch->s = ch->k_dot_a + ch->root * ch->d_dot_a;
		if (fabs(ch->s) > ch->half_h)
			return (0);
	}
	ch->point = vec3_multiply_inline(&r->direction, ch->root);
	ch->point = vec3_add_inline(&r->origin, &ch->point);
	return (1);
}

/**
 * Computes ray intersection with finite open cylinder (no caps)
 * Cylinder defined by center, axis (norm), diameter, and height
 * Solves quadratic in plane perpendicular to axis
 * Validates t interval and height constraint
 * Fills hit record with t, point, normal, color, and material
 * Returns 1 on hit, 0 otherwise
 */
int	hit_cylinder_obj(const t_cylinder *cyl, t_ray *r,
	double tmin, double tmax, t_hit_record *rec)
{
	t_cyl_hit	ch;

	if (!cyl || !rec)
		return (0);
	if (!cyl_valid_axis(cyl, &ch))
		return (0);
	ch.radius = cyl->diameter * 0.5;
	ch.half_h = cyl->height * 0.5;
	ch.k = vec3_sub_inline(&r->origin, &cyl->cords);
	ch.d_dot_a = vec3_dot_inline(&r->direction, &ch.axis);
	ch.k_dot_a = vec3_dot_inline(&ch.k, &ch.axis);
	ch.a = vec3_dot_inline(&r->direction, &r->direction)
		- ch.d_dot_a * ch.d_dot_a;
	if (ch.a == 0.0)
		return (0);
	ch.h = vec3_dot_inline(&r->direction, &ch.k) - ch.d_dot_a * ch.k_dot_a;
	ch.c = vec3_dot_inline(&ch.k, &ch.k) - ch.k_dot_a * ch.k_dot_a
		- ch.radius * ch.radius;
	ch.disc = ch.h * ch.h - ch.a * ch.c;
	if (ch.disc < 0.0)
		return (0);
	if (!cyl_select_root(&ch, r, tmin, tmax))
		return (0);
	ch.v = vec3_sub_inline(&ch.point, &cyl->cords);
	ch.axis_part = vec3_multiply_inline(&ch.axis,
			vec3_dot_inline(&ch.v, &ch.axis));
	ch.outward = vec3_sub_inline(&ch.v, &ch.axis_part);
	ch.len = vec3_sqrt(&ch.outward);
	if (ch.len != 0.0)
		ch.outward = vec3_divide_inline(&ch.outward, ch.len);
	rec->t = ch.root;
	rec->p = ch.point;
	set_face_normal(rec, r, &ch.outward);
	rec->rgb = cyl->rgb;
	rec->mat = cyl->mat;
	return (1);
}
