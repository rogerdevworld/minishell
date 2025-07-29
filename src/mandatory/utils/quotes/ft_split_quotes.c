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
#include "../../../../include/minishell.h"

static void	ft_quotes(char c, int *in_quotes, char *quote_type)
{
	int	is_quote;

	is_quote = (c == '"' || c == '\'');
	if (!is_quote)
		return ;
	if (!(*in_quotes) || *quote_type == c)
	{
		*in_quotes = !(*in_quotes);
		if (*in_quotes)
			*quote_type = c;
		else
			*quote_type = 0;
	}
}

static int	count_words_loop(const char *s, char c)
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
				ft_quotes(*s, &in_quotes, &quote_type);
				s++;
			}
		}
	}
	return (count);
}

static char	*word_dup_loop(const char *s, char c)
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
		ft_quotes(s[len], &in_quotes, &quote_type);
		len++;
	}
	word = (char *)malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	strncpy(word, s, len);
	word[len] = '\0';
	return (word);
}

static int	process_split_loop(char **s, char **split, char c, int *i)
{
	int		in_quotes;
	char	quote_type;

	in_quotes = 0;
	quote_type = 0;
	while (**s)
	{
		while (**s == c && !in_quotes)
			(*s)++;
		if (**s)
		{
			split[*i] = word_dup_loop(*s, c);
			if (!split[*i])
				return (0);
			(*i)++;
			while (**s && (**s != c || in_quotes))
			{
				ft_quotes(**s, &in_quotes, &quote_type);
				(*s)++;
			}
		}
	}
	return (1);
}

char	**ft_split_quotes(char const *s, char c)
{
	char	**split;
	int		words;
	int		i;

	if (!s)
		return (NULL);
	words = count_words_loop(s, c);
	split = (char **)malloc(sizeof(char *) * (words + 1));
	if (!split)
		return (NULL);
	i = 0;
	if (!process_split_loop((char **)&s, split, c, &i))
	{
		free_split(split);
		return (NULL);
	}
	split[i] = NULL;
	return (split);
}
