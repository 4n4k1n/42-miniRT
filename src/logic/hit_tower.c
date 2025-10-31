#include "mini_rt.h"

/**
 * Normalizes the cylinder axis vector
 * Stores normalized axis and its original length
 * Returns 0 if axis vector length is zero, else 1
 */
static int	cyl_valid_axis(const t_cylinder *cyl, t_cyl_hit *ch)
{
	ch->axis_len = vec3_sqrt(cyl->norm);
	if (ch->axis_len == 0.0)
		return (0);
	ch->axis = vec3_normalize(cyl->norm);
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
 * Intersects the ray with the cylinder side surface
 * Fills t_cyl_hit with root, point, s and outward normal when valid
 * Returns 1 on success, 0 otherwise
 */
static int	cyl_side_hit(const t_cylinder *cyl, t_ray *r, double tmin, double tmax, t_cyl_hit *ch)
{
	t_vec3	d_perp;
	t_vec3	k_perp;
	t_vec3	q;

	ch->radius = cyl->diameter * 0.5;
	ch->half_h = cyl->height * 0.5;
	ch->k = vec3_sub(r->origin, cyl->cords);
	ch->d_dot_a = vec3_dot(r->direction, ch->axis);
	ch->k_dot_a = vec3_dot(ch->k, ch->axis);
	d_perp = vec3_sub(r->direction, vec3_multiply(ch->axis, ch->d_dot_a));
	k_perp = vec3_sub(ch->k, vec3_multiply(ch->axis, ch->k_dot_a));
	ch->a = vec3_dot(d_perp, d_perp);
	ch->h = vec3_dot(d_perp, k_perp);
	ch->c = vec3_dot(k_perp, k_perp) - ch->radius * ch->radius;
	ch->disc = ch->h * ch->h - ch->a * ch->c;
	if (ch->a <= 1e-12 || ch->disc < 0.0)
		return (0);
	if (!cyl_select_root(ch, r, tmin, tmax))
		return (0);
	q = vec3_sub(ch->point, cyl->cords);
	ch->outward = vec3_sub(q, vec3_multiply(ch->axis, vec3_dot(q, ch->axis)));
	ch->outward = vec3_normalize(ch->outward);
	return (1);
}

/**
 * Computes cylinder intersection and returns the closest hit among side/caps
 * Computes UV and tangent basis at the hit for bump mapping
 * Returns 1 if hit, 0 if miss
 */
int	hit_cylinder_obj(const t_cylinder *cyl, t_ray *r, double tmin, double tmax, t_hit_record *rec)
{
	t_vec3			best_p;
	t_vec3			best_n;
	double			best_t;
	int				best_hit;
	t_cyl_hit		ch;
	t_vec3			a;
	t_vec3			tan;
	t_vec3			tmp;
	t_vec3			base;
	t_vec3			k;
	double			s;
	double			u;
	double			v;

	best_hit = 0;
	best_t = tmax;
	if (!cyl_valid_axis(cyl, &ch))
		return (0);
	a = ch.axis;
	if (cyl_side_hit(cyl, r, tmin, tmax, &ch)
		&& ch.root >= tmin && ch.root <= tmax)
	{
		best_hit = 1;
		best_t = ch.root;
		best_p = ch.point;
		best_n = vec3_normalize(vec3_sub(ch.point, vec3_add(cyl->cords, vec3_multiply(a, ch.s))));
	}
	if (cyl_cap_hit(cyl, r, &ch, 1)
		&& ch.root >= tmin && ch.root <= tmax
		&& ch.root < best_t)
	{
		best_hit = 1;
		best_t = ch.root;
		best_p = ch.point;
		best_n = a;
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
	tmp = (fabs(a.y) < 0.999) ? (t_vec3){0.0, 1.0, 0.0} : (t_vec3){1.0, 0.0, 0.0};
	tan = vec3_normalize(vec3_cross(tmp, a));
	rec->tangent = vec3_normalize(vec3_cross(a, vec3_normalize(vec3_sub(best_p, vec3_add(cyl->cords, vec3_multiply(a, vec3_dot(vec3_sub(best_p, cyl->cords), a)))))));
	rec->bitangent = a;
	base = vec3_sub(cyl->cords, vec3_multiply(a, cyl->height * 0.5));
	k = vec3_sub(best_p, base);
	s = fmax(0.0, fmin(cyl->height, vec3_dot(k, a)));
	{
		t_vec3 rp = vec3_normalize(vec3_sub(k, vec3_multiply(a, s)));
		double x = vec3_dot(rp, tan);
		double y = vec3_dot(rp, vec3_cross(a, tan));
		u = (atan2(y, x) + M_PI) / (2.0 * M_PI);
		v = s / cyl->height;
	}
	rec->u = u;
	rec->v = v;
	rec->bump = cyl->bump;
	set_face_normal(rec, r, &best_n);
	rec->rgb = cyl->rgb;
	rec->mat = cyl->mat;
	return (1);
}
