#include "mini_rt.h"

int	vec3_near_zero(const t_vec3 *v)
{
	const double s = 1e-8;
	return (fabs(v->x) < s) && (fabs(v->y) < s) && (fabs(v->z) < s);
}