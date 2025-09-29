#ifndef MATH_H
# define MATH_H

# include "mini_rt.h"

// member operations
t_vec3	*vec3_add_member(t_vec3 *dst, t_vec3 *src);
t_vec3	*vec3_sub_member(t_vec3 *dst, t_vec3 *src);
t_vec3	*vec3_overload_member(t_vec3 *vec);
t_vec3	*vec3_multiply(t_vec3 *vec, double t);
t_vec3	*vec3_divide(t_vec3 *vec, double t);
double	vec3_squared(t_vec3 *vec);
double	vec3_sqrt(t_vec3 *vec);

// external operations
// (added declarations for all functions implemented in src/math/)
t_vec3	*vec3_add(t_vec3 *dst, t_vec3 *src);
t_vec3	vec3_add_inline(t_vec3 *dst, t_vec3 *src);

t_vec3	*vec3_sub(t_vec3 *dst, t_vec3 *src);
t_vec3	*vec3_sub_inline(t_vec3 *dst, t_vec3 *src);

t_vec3	*vec3_cpy(t_vec3 *dst, t_vec3 *src);
t_vec3	vec3_cpy_inline(t_vec3 *vec);

t_vec3	*vec3_overload(t_vec3 *vec);
t_vec3	vec3_overload_inline(t_vec3 *vec);

t_vec3	*vec3_multiply(t_vec3 *vec, double t);
t_vec3	vec3_multiply_inline(t_vec3 *vec, double t);

t_vec3	*vec3_divide(t_vec3 *vec, double t);
t_vec3	vec3_divide_inline(t_vec3 *vec, double t);

t_vec3	vec3_cross_inline(t_vec3 *a, t_vec3 *b);

double	vec3_dot_inline(t_vec3 *a, t_vec3 *b);

#endif