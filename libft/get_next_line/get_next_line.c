/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 16:20:08 by nweber            #+#    #+#             */
/*   Updated: 2025/08/24 17:47:57 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*ft_read(int fd, char *buffer, char *line, int *b_read)
{
	char	*temp;

	*b_read = 1;
	while (!ft_strchr(buffer, '\n') && *b_read != 0)
	{
		*b_read = read(fd, buffer, BUFFER_SIZE);
		if (*b_read == -1)
			return (ft_memcpy(buffer, "\0", 1), free(line), NULL);
		buffer[*b_read] = '\0';
		temp = ft_strjoin(line, buffer);
		if (!temp)
			return (free(line), NULL);
		free(line);
		line = temp;
	}
	return (line);
}

static char	*extract_line(char *line)
{
	size_t	i;

	i = 0;
	while (line[i] && line[i] != '\n')
		i++;
	if (line[i] == '\n')
		return (ft_substr(line, 0, i + 1));
	else
		return (ft_strdup(line));
}

static void	buffer_clean(char *buffer)
{
	size_t	i;

	i = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (buffer[i] == '\n')
		ft_memcpy(buffer, ft_strchr(buffer, '\n') + 1,
			ft_strlen(ft_strchr(buffer, '\n')));
}

char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE + 1];
	int			b_read;
	char		*line;
	char		*temp;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (ft_memcpy(buffer, "\0", 1), NULL);
	line = ft_strdup(buffer);
	if (!line)
		return (NULL);
	line = ft_read(fd, buffer, line, &b_read);
	if (!line)
		return (NULL);
	if (ft_strlen(line) == 0 && b_read == 0)
		return (free(line), NULL);
	temp = extract_line(line);
	return (buffer_clean(buffer), free(line), temp);
}
