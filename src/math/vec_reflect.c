#include "mini_rt.h"

t_vec3	vec3_reflect_inline(const t_vec3 *v, const t_vec3 *n)
{
	const double d = 2.0 * vec3_dot_inline(v, n);
	t_vec3 twice_n = vec3_multiply_inline(n, d);
	return vec3_sub_inline(v, &twice_n);
}
