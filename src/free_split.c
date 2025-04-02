/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:36:52 by rmarrero          #+#    #+#             */
/*   Updated: 2025/03/27 17:36:53 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/minishell.h"

char	*ft_strncpy(char *dest, const char *src, unsigned int n)
{
	unsigned int	i;

	i = 0;
	while (i < n && src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}

static int	count_words(const char *s, char c)
{
	int		count;
	int		in_quotes;
	char	quote_type;

	count = 0;
	in_quotes = 0;
	quote_type = 0;
	while (*s)
	{
		while (*s == c && !in_quotes)
			s++;
		if (*s)
		{
			count++;
			while (*s && (*s != c || in_quotes))
			{
				if ((*s == '"' || *s == '\'') && (!in_quotes
						|| quote_type == *s))
				{
					in_quotes = !in_quotes;
					if (in_quotes)
						quote_type = *s;
					else
						quote_type = 0;
				}
				s++;
			}
		}
	}
	return (count);
}

static char	*ft_word_dup(const char *s, char c)
{
	unsigned int	len;
	int				in_quotes;
	char			quote_type;
	char			*word;

	len = 0;
	in_quotes = 0;
	quote_type = 0;
	while (s[len] && (s[len] != c || in_quotes))
	{
		if ((s[len] == '"' || s[len] == '\'') && (!in_quotes
				|| quote_type == s[len]))
		{
			in_quotes = !in_quotes;
			if (in_quotes)
				quote_type = s[len];
			else
				quote_type = 0;
		}
		len++;
	}
	word = (char *)malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	strncpy(word, s, len);
	word[len] = '\0';
	return (word);
}

void	free_split(char **split)
{
	int	i;

	i = 0;
	if (!split)
		return ;
	while (split[i])
		free(split[i++]);
	free(split);
}

char	**ft_split_full(char const *s, char c)
{
	int		words;
	char	**split;
	int		i;
	int		in_quotes;
	char	quote_type;

	words = count_words(s, c);
	split = (char **)malloc(sizeof(char *) * (words + 1));
	i = 0;
	in_quotes = 0;
	quote_type = 0;
	if (!split)
		return (NULL);
	while (*s)
	{
		while (*s == c && !in_quotes)
			s++;
		if (*s)
		{
			split[i] = ft_word_dup(s, c);
			if (!split[i])
				return (free_split(split), NULL);
			i++;
			while (*s && (*s != c || in_quotes))
			{
				if ((*s == '"' || *s == '\'') && (!in_quotes
						|| quote_type == *s))
				{
					in_quotes = !in_quotes;
					if (in_quotes)
						quote_type = *s;
					else
						quote_type = 0;
				}
				s++;
			}
		}
	}
	split[i] = NULL;
	return (split);
}
