/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_quotes_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaacosta <jaacosta@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 13:39:46 by jaacosta          #+#    #+#             */
/*   Updated: 2025/07/22 13:39:47 by jaacosta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../../include/minishell.h"

/**
 * Removes outer double quotes from a string if both start and 
	end with double quotes.
 * If the string does not meet this condition, or if it's NULL,
	a duplicate of the original string is returned.
 * This function specifically targets strings like "hello" to become hello,
	but 'hello' remains 'hello'.
 * @param str The input string from which to remove outer double quotes.
 * @return A newly allocated string with outer double quotes removed,
	or a duplicate of the original string. Returns NULL if the input is NULL.
 */
char	*remove_quotes(char *str)
{
	size_t	len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if (len >= 2)
	{
		if ((str[0] == '"' && str[len - 1] == '"')
			|| (str[0] == '\'' && str[len - 1] == '\''))
		{
			return (ft_substr(str, 1, len - 2));
		}
	}
	return (ft_strdup(str));
}

/**
 * Removes all single and double quotes from a string.
 * It iterates through the string,
	tracking the state of open quotes to correctly
 * skip the quote characters themselves while including the content within them.
 * For example, "hello'world'" becomes helloworld.
 * @param str The input string from which to remove all quotes.
 * @return A newly allocated string with all quotes removed. 
	Returns NULL if memory allocation fails.
 */
char	*remove_all_quotes(char *str)
{
	char	*result;
	int		i;
	int		j;
	char	quote;

	i = 0;
	j = 0;
	quote = 0;
	result = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	if (!result)
		return (NULL);
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"') && !quote)
			quote = str[i++];
		else if (str[i] == quote)
		{
			quote = 0;
			i++;
		}
		else
			result[j++] = str[i++];
	}
	return (result);
}

/**
 * Helper function to allocate memory for a new array of strings,
 * which will hold the quote-removed versions of the original arguments.
 * It calculates the count of existing arguments and allocates 
	space for them plus a NULL terminator.
 * @param args The original array of strings.
 * @param count A pointer to an integer that will store 
	the number of strings in `args`.
 * @return A newly allocated `char**` array initialized to zeros,
	or NULL if memory allocation fails.
 */
static char	**allocate_args_no_quotes(char **args, int *count)
{
	char	**args_no_quotes;

	*count = 0;
	while (args && args[*count])
		(*count)++;
	args_no_quotes = ft_calloc(*count + 1, sizeof(char *));
	if (!args_no_quotes)
		return (NULL);
	return (args_no_quotes);
}

/**
 * Creates a new array of strings where all quotes (single and double) 
	have been removed from each original argument.
 * It iterates through the input `args` array,
	applies `remove_all_quotes` to each string,
 * and populates a new array with the results. Handles memory allocation 
	and cleanup in case of failure.
 * @param args The original null-terminated array of strings.
 * @return A newly allocated null-terminated `char**` array with all quotes
	removed from its strings.
 * Returns NULL if memory allocation fails at any point.
 */
char	**remove_quotes_from_args(char **args)
{
	int		count;
	int		i;
	int		j;
	char	**args_no_quotes;

	args_no_quotes = allocate_args_no_quotes(args, &count);
	if (!args_no_quotes)
		return (NULL);
	i = 0;
	j = 0;
	while (i < count)
	{
		args_no_quotes[i] = remove_all_quotes(args[i]);
		if (!args_no_quotes[i])
		{
			while (j < i)
				free(args_no_quotes[j++]);
			free(args_no_quotes);
			return (NULL);
		}
		i++;
	}
	args_no_quotes[count] = NULL;
	return (args_no_quotes);
}
