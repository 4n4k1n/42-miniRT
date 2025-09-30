/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 17:28:26 by nweber            #+#    #+#             */
/*   Updated: 2025/09/30 11:01:05 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// fd = open(av[1], O_RDONLY)
// while(1)
// 	read line with gnl until newline-> validate the line
// 		-> if valid safe in data struct
// Capitalized Elements are only used once
// Random amount of whitespaces and newlines
// First info is always the identifier
//

static char	*trim_and_strip(char *s)
{
	char	*hash;
	char	*out;

	hash = ft_strchr(s, '#');
	if (hash)
		*hash = '\0';
	out = ft_strtrim(s, " \t\r\n");
	return (out);
}

static int	init_scene_lists(t_main *scene)
{
	scene->objects = (t_obj_list *)malloc(sizeof(t_obj_list));
	if (!scene->objects)
		return (rt_error("malloc failed (objects)"));
	scene->light_list = (t_light_list *)malloc(sizeof(t_light_list));
	if (!scene->light_list)
		return (rt_error("malloc failed (lights)"));
	scene->objects->head = NULL;
	scene->objects->tail = NULL;
	scene->objects->size = 0;
	scene->light_list->head = NULL;
	scene->light_list->tail = NULL;
	scene->light_list->size = 0;
	scene->ambiente.lighting = -1.0;
	scene->ambiente.color.r = -1;
	scene->camera.fov = -1;
	return (0);
}

static int	dispatch_line(char **tokens, t_main *scene, t_arg_check *args)
{
	if (!tokens || !tokens[0] || tokens[0][0] == '\0')
		return (0);
	if (ft_strcmp(tokens[0], "A") == 0)
		return (parse_ambient(tokens, scene, args));
	else if (ft_strcmp(tokens[0], "C") == 0)
		return (parse_camera(tokens, scene, args));
	else if (ft_strcmp(tokens[0], "L") == 0)
		return (parse_light(tokens, scene, args));
	else if (ft_strcmp(tokens[0], "sp") == 0)
		return (parse_sphere(tokens, scene));
	else if (ft_strcmp(tokens[0], "pl") == 0)
		return (parse_plane(tokens, scene));
	else if (ft_strcmp(tokens[0], "cy") == 0)
		return (parse_cylinder(tokens, scene));
	return (rt_error("invalid identifier"));
}

static int	read_parse(int fd, t_main *scene, t_arg_check *args)
{
	char	*raw;
	char	*line;
	char	**tokens;

	args->has_a = 0;
	args->has_c = 0;
	args->has_l = 0;
	raw = get_next_line(fd);
	while (raw)
	{
		line = trim_and_strip(raw);
		free(raw);
		if (!line)
			return (rt_error("malloc error"));
		if (line[0] != '\0')
		{
			tokens = ft_split(line, ' ');
			if (!tokens)
				return (free(line), rt_error("malloc error"));
			if (dispatch_line(tokens, scene, args))
				return (ft_array_free(tokens), free(line), 1);
			ft_array_free(tokens);
		}
		free(line);
		raw = get_next_line(fd);
	}
	if (!args->has_a || !args->has_c || !args->has_l)
		return (rt_error("missing elements"));
	return (0);
}

int	parse_scene(const char *path, t_main *scene)
{
	int			fd;
	t_arg_check	args;

	if (!path || !scene)
		return (rt_error("invalid arguments"));
	if (!is_rt_file(path))
		return (rt_error("invalid file(expected .rt)"));
	if (init_scene_lists(scene))
		return (rt_error("initialization error"));
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (rt_error("failed to open file"));
	if (read_parse(fd, scene, &args))
		return (close(fd), rt_error("parsing error"));
	close(fd);
	return (0);
}
