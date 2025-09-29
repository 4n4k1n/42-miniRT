#ifndef PARSING_H
# define PARSING_H

#include "objects.h"

typedef struct	s_arg_check
{
	int	has_a;
	int	has_c;
	int	has_l;
}		t_arg_check;


// Error return
int	rt_error(const char *msg);

// Parsing
int	parse_scene(const char *path, t_main *scene);

#endif