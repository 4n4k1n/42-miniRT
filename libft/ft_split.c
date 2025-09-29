/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 15:40:11 by nweber            #+#    #+#             */
/*   Updated: 2025/07/07 13:12:52 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// splits a given str into multiple strings splitted by a char
static int	count_words(const char *s, char c)
{
	size_t	i;
	size_t	word_count;

	word_count = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] != c)
		{
			word_count++;
			while (s[i] && s[i] != c)
				i++;
		}
		else
			i++;
	}
	return (word_count);
}

static char	*get_word(const char *s, char c, size_t *i)
{
	size_t	start;
	char	*word;

	while (s[*i] == c)
		(*i)++;
	start = *i;
	while (s[*i] && s[*i] != c)
		(*i)++;
	word = (char *)malloc(*i - start + 1);
	if (!word)
		return (NULL);
	ft_strlcpy(word, s + start, *i - start + 1);
	return (word);
}

static char	**fill_words(char **words, const char *s, char c)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (s[i])
	{
		if (s[i] != c)
		{
			words[j] = get_word(s, c, &i);
			if (!words[j])
			{
				while (j > 0)
					free(words[--j]);
				free(words);
				return (NULL);
			}
			j++;
		}
		else
			i++;
	}
	words[j] = NULL;
	return (words);
}

char	**ft_split(const char *s, char c)
{
	char	**words;

	if (!s)
		return (NULL);
	words = (char **)malloc(sizeof(char *) * (count_words(s, c) + 1));
	if (!words)
		return (NULL);
	return (fill_words(words, s, c));
}
