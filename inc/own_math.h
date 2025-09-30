#ifndef OWN_MATH_H
# define OWN_MATH_H

# include "objects.h"

// member operations
t_vec3	*vec3_add_member(t_vec3 *dst, t_vec3 *src);
t_vec3	*vec3_sub_member(t_vec3 *dst, t_vec3 *src);
t_vec3	*vec3_overload_member(t_vec3 *vec);
t_vec3	*vec3_multiply(t_vec3 *vec, double t);
t_vec3	*vec3_divide(t_vec3 *vec, double t);
double	vec3_squared(const t_vec3 *vec);
double	vec3_sqrt(const t_vec3 *vec);

// external operations
t_vec3	*vec3_add(t_vec3 *dst, t_vec3 *src);
t_vec3	vec3_add_inline(const t_vec3 *dst, const t_vec3 *src);

t_vec3	*vec3_sub(t_vec3 *dst, t_vec3 *src);
t_vec3	vec3_sub_inline(const t_vec3 *dst, const t_vec3 *src);

t_vec3	*vec3_cpy(t_vec3 *dst, t_vec3 *src);
t_vec3	vec3_cpy_inline(const t_vec3 *vec);

t_vec3	*vec3_overload(t_vec3 *vec);
t_vec3	vec3_overload_inline(const t_vec3 *vec);

t_vec3	*vec3_multiply(t_vec3 *vec, double t);
t_vec3	vec3_multiply_inline(const t_vec3 *vec, double t);

t_vec3	*vec3_divide(t_vec3 *vec, double t);
t_vec3	vec3_divide_inline(const t_vec3 *vec, double t);

t_vec3	vec3_cross_inline(const t_vec3 *a, const t_vec3 *b);

double	vec3_dot_inline(const t_vec3 *a, const t_vec3 *b);

t_vec3	vec3_init_inline(double x, double y, double z);

#endif