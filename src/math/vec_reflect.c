#include "mini_rt.h"

t_vec3	vec3_reflect_inline(const t_vec3 *v, const t_vec3 *n)
{
	double d;
	t_vec3 twice_n;

	d = 2.0 * vec3_dot_ptr(v, n);
	twice_n = vec3_multiply_ptr(n, d);
	return (vec3_sub_ptr(v, &twice_n));
}