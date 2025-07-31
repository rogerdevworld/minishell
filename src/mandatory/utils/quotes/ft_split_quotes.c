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

/**
 * Toggles the `in_quotes` flag and sets/resets `quote_type` 
	based on encountered quote characters.
 * This helper function is crucial for `ft_split_quotes` to 
	correctly identify text within quotes,
 * ensuring that delimiters inside quotes are not treated as separators.
 *
 * @param c The character being examined.
 * @param in_quotes A pointer to an integer flag (0 for false,
	1 for true) indicating if currently inside quotes.
 * @param quote_type A pointer to a character storing the 
	type of the current open quote (' or ").
 */
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

/**
 * Counts the number of "words" (substrings) in a string,
	considering a delimiter character
 * and ignoring delimiters that are inside single or double quotes.
 * This function is used by `ft_split_quotes` to 
	determine the size of the array needed.
 * @param s The input string to count words in.
 * @param c The delimiter character.
 * @return The total count of words found.
 */
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

/**
 * Duplicates a "word" (substring) from the input string, stopping at the first
 * occurrence of the delimiter `c` that is NOT inside quotes.
 * @param s The input string from which to duplicate the word.
 * @param c The delimiter character.
 * @return A newly allocated string containing the duplicated word,
	or NULL if memory allocation fails.
 */
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

/**
 * Internal helper function for `ft_split_quotes` to process the input string
 * and populate the `split` array with extracted words. It handles skipping
 * delimiters and extracting words while respecting quoted sections.
 * @param s A pointer to the current position in the input 
	string. This pointer will be advanced.
 * @param split The array of strings to populate with extracted words.
 * @param c The delimiter character.
 * @param i A pointer to the current index in the `split` array,
	which will be incremented.
 * @return 1 on success, 0 if memory allocation for a word fails.
 */
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

/**
 * Splits a string `s` by a delimiter character `c`, while correctly handling
 * single and double quotes. Delimiters inside quotes are ignored.
 *
 * @param s The string to be split.
 * @param c The delimiter character.
 * @return A null-terminated array of newly allocated strings (the "words"),
 * or NULL if memory allocation fails at any point 
	or if the input string is NULL.
 */
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
