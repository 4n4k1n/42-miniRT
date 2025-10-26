#include "mini_rt.h"

/**
 * Normalizes the cylinder axis vector
 * Stores normalized axis and its original length
 * Returns 0 if axis vector length is zero, else 1
 */
static int	cyl_valid_axis(const t_cylinder *cyl, t_cyl_hit *ch)
{
	ch->axis = cyl->norm;
	ch->axis_len = vec3_sqrt(ch->axis);
	if (ch->axis_len == 0.0)
		return (0);
	ch->axis = vec3_divide(ch->axis, ch->axis_len);
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
	ch->point = vec3_multiply(r->direction, ch->root);
	ch->point = vec3_add(r->origin, ch->point);
	return (1);
}

/**
 * Tests intersection with a single cylinder cap
 * top = 1 for top cap, 0 for bottom
 * Writes hit t, point, and normal into ch on success
 * Returns 1 if the cap is hit within bounds, else 0
 */
static int	cyl_cap_hit(const t_cylinder *cyl, t_ray *r,
	t_cyl_hit *ch, int top)
{
	t_vec3	center;
	t_vec3	tmp;
	double	sign;
	double	denom;
	double	t;
	t_vec3	cp;
	double	dist2;

	sign = top ? 1.0 : -1.0;
	center = vec3_multiply(ch->axis, ch->half_h * sign);
	center = vec3_add(cyl->cords, center);
	denom = vec3_dot(r->direction, ch->axis) * sign;
	if (fabs(denom) < 1e-9)
		return (0);
	tmp = vec3_sub(center, r->origin);
	t = vec3_dot(tmp, ch->axis) * sign / denom;
	if (t <= 0.0)
		return (0);
	cp = vec3_multiply(r->direction, t);
	cp = vec3_add(r->origin, cp);
	tmp = vec3_sub(cp, center);
	dist2 = vec3_dot(tmp, tmp);
	if (dist2 > ch->radius * ch->radius)
		return (0);
	ch->root = t;
	ch->point = cp;
	ch->outward = ch->axis;
	if (!top)
		ch->outward = vec3_multiply(ch->outward, -1.0);
	return (1);
}

/**
 * Computes ray intersection with finite closed cylinder (with caps)
 * Resolves side surface and both caps selecting nearest valid hit
 * Fills hit record with t, point, normal, color, and material
 * Returns 1 on hit, 0 otherwise
 */
int	hit_cylinder_obj(const t_cylinder *cyl, t_ray *r,
	double tmin, double tmax, t_hit_record *rec)
{
	t_cyl_hit	ch;
	int			has_side;
	int			best_hit;
	double		best_t;
	t_vec3		best_p;
	t_vec3		best_n;
	double		side_t;
	t_vec3		side_p;
	t_vec3		side_n;

	if (!cyl || !rec)
		return (0);
	if (!cyl_valid_axis(cyl, &ch))
		return (0);
	ch.radius = cyl->diameter * 0.5;
	ch.half_h = cyl->height * 0.5;
	ch.k = vec3_sub(r->origin, cyl->cords);
	ch.d_dot_a = vec3_dot(r->direction, ch.axis);
	ch.k_dot_a = vec3_dot(ch.k, ch.axis);
	ch.a = vec3_dot(r->direction, r->direction)
		- ch.d_dot_a * ch.d_dot_a;
	has_side = 0;
	if (ch.a != 0.0)
	{
		ch.h = vec3_dot(r->direction, ch.k)
			- ch.d_dot_a * ch.k_dot_a;
		ch.c = vec3_dot(ch.k, ch.k) - ch.k_dot_a * ch.k_dot_a
			- ch.radius * ch.radius;
		ch.disc = ch.h * ch.h - ch.a * ch.c;
		if (ch.disc >= 0.0 && cyl_select_root(&ch, r, tmin, tmax))
		{
			ch.v = vec3_sub(ch.point, cyl->cords);
			ch.axis_part = vec3_multiply(ch.axis,
					vec3_dot(ch.v, ch.axis));
			ch.outward = vec3_sub(ch.v, ch.axis_part);
			ch.len = vec3_sqrt(ch.outward);
			if (ch.len != 0.0)
				ch.outward = vec3_divide(ch.outward, ch.len);
			side_t = ch.root;
			side_p = ch.point;
			side_n = ch.outward;
			has_side = 1;
		}
	}
	best_hit = 0;
	best_t = tmax + 1.0;
	if (has_side && side_t >= tmin && side_t <= tmax)
	{
		best_hit = 1;
		best_t = side_t;
		best_p = side_p;
		best_n = side_n;
	}
	if (cyl_cap_hit(cyl, r, &ch, 1)
		&& ch.root >= tmin && ch.root <= tmax
		&& ch.root < best_t)
	{
		best_hit = 1;
		best_t = ch.root;
		best_p = ch.point;
		best_n = ch.outward;
	}
	if (cyl_cap_hit(cyl, r, &ch, 0)
		&& ch.root >= tmin && ch.root <= tmax
		&& ch.root < best_t)
	{
		best_hit = 1;
		best_t = ch.root;
		best_p = ch.point;
		best_n = ch.outward;
	}
	if (!best_hit)
		return (0);
	rec->t = best_t;
	rec->p = best_p;
	set_face_normal(rec, r, &best_n);
	rec->rgb = cyl->rgb;
	rec->mat = cyl->mat;
	return (1);
}
