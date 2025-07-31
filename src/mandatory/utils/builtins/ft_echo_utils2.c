/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaacosta <jaacosta@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 21:39:27 by jaacosta          #+#    #+#             */
/*   Updated: 2025/07/21 21:39:28 by jaacosta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../../include/minishell.h"

/**
 * Handles quoted sections within an argument string for expansion.
 * It checks if the current character at `arg[*i]` is a single or double quote.
 * If it's a single quote, it calls `copy_single_quoted_text`.
 * If it's a double quote,
	it calls `copy_double_quoted_text` (which handles inner expansions).
 * Returns a newly allocated string containing the content 
	from inside the quotes,
	or NULL if no quote is found.
 */
static char	*handle_quotes(t_minishell *minishell, char *arg, int *i, int s)
{
	if (arg[*i] == '\'')
		return (copy_single_quoted_text(arg, i));
	else if (arg[*i] == '"')
		return (copy_double_quoted_text(minishell, arg, i, s));
	return (NULL);
}

/**
 * Handles escaped characters within an argument string.
 * Specifically deals with `\$` (to treat '$' literally) and 
	other escaped characters.
 * Increments the index `*i` past the escaped sequence.
 * Returns a newly allocated string containing the escaped character,
	or NULL if no escape sequence is found.
 */
static char	*handle_escapes(char *arg, int *i)
{
	char	*part;

	part = NULL;
	if (arg[*i] == '\\' && arg[*i + 1] == '$')
	{
		part = ft_substr(arg, *i + 1, 1);
		*i += 2;
	}
	else if (arg[*i] == '\\')
	{
		if (arg[*i + 1])
		{
			part = ft_substr(arg, *i + 1, 1);
			*i += 2;
		}
		else
		{
			part = ft_strdup("\\");
			(*i)++;
		}
	}
	return (part);
}

/**
 * Processes a segment of an argument string, handling various types of content.
 * It attempts to handle quotes, then escape sequences,
	then variable expansions,
 * and finally plain text. It calls the appropriate helper
	function based on the character
 * at the current index `*i`.
 * Returns a newly allocated string containing the processed segment.
 */
char	*process_segment(t_minishell *minishell, char *arg, int *i, int s)
{
	char	*part;

	part = NULL;
	part = handle_quotes(minishell, arg, i, s);
	if (part)
		return (part);
	part = handle_escapes(arg, i);
	if (part)
		return (part);
	if (arg[*i] == '$')
		return (expand_variable(minishell, arg, i, s));
	return (copy_plain_text(arg, i));
}

/**
 * Joins two strings and frees the memory of both original strings.
 * This is a utility function to concatenate parts during argument expansion.
 * Returns a newly allocated string with the concatenated content.
 */
char	*join_and_free(char *result, char *part)
{
	char	*joined;

	joined = ft_strjoin_free(result, part);
	return (joined);
}

/**
 * Prints the arguments for the 'echo' command to standard output.
 * It iterates through the arguments starting from a specified index,
 * expands each argument (handling quotes and variables), prints it,
 * and adds a space between arguments.
 * Returns 0 on success, or 1 if any memory allocation fails during expansion.
 */
int	print_echo_args(t_minishell *minishsell, char **args, int start, int s)
{
	int		i;
	char	*expanded;

	i = start;
	while (args[i])
	{
		expanded = ft_expand_arg(minishsell, args[i], s);
		if (!expanded)
			return (1);
		ft_printf("%s", expanded);
		free(expanded);
		if (args[i + 1])
			write(1, " ", 1);
		i++;
	}
	return (0);
}
