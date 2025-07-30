/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaacosta <jaacosta@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 21:26:14 by jaacosta          #+#    #+#             */
/*   Updated: 2025/07/21 21:26:16 by jaacosta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../../include/minishell.h"

/**
 * Joins two strings and frees the memory of both original strings.
 * This is a utility function to concatenate strings and 
	manage memory efficiently.
 * Returns a newly allocated string containing the joined content.
 */
char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*joined;

	joined = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (joined);
}

/**
 * Expands an environment variable's value for the 'echo' command.
 * It searches for the given key in the environment list and
	returns a duplicate of its content.
 * Returns a newly allocated string with the variable's value,
	or NULL if the key is not found.
 */
char	*ft_echo_expand(char *str, t_env *env)
{
	while (env)
	{
		if (ft_strcmp(str, env->key) == 0)
			return (ft_strdup(env->content));
		env = env->next;
	}
	return (NULL);
}

/**
 * Checks if a string is a valid '-n' flag for the 'echo' command.
 * A valid flag starts with '-' and is followed by one or more 'n' characters.
 * Returns 1 if it's a valid '-n' flag, 0 otherwise.
 */
int	is_valid_n_flag(char *str)
{
	int	i;

	if (!str || str[0] != '-')
		return (0);
	i = 1;
	if (str[i] == '\0')
		return (0);
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

/**
 * Copies a segment of plain text from an argument string.
 * It reads characters until a '$', single quote, double quote, backslash,
	or null terminator is encountered.
 * Increments the index 'i' past the copied text.
 * Returns a newly allocated substring containing the plain text.
 */
char	*copy_plain_text(char *arg, int *i)
{
	int	start;

	start = *i;
	while (arg[*i] && arg[*i] != '$' && arg[*i] != '\'' && arg[*i] != '"'
		&& arg[*i] != '\\')
		(*i)++;
	return (ft_substr(arg, start, *i - start));
}

/**
 * Copies content from within single quotes.
 * It extracts all characters between the opening and closing single quotes.
 * Increments the index 'i' past the closing single quote.
 * Returns a newly allocated substring with the text inside the single quotes.
 */
char	*copy_single_quoted_text(const char *arg, int *i)
{
	char	*text;
	int		start;

	start = ++(*i);
	while (arg[*i] && arg[*i] != '\'')
		(*i)++;
	text = ft_substr(arg, start, *i - start);
	if (arg[*i] == '\'')
		(*i)++;
	return (text);
}
