/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansor_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaacosta <jaacosta@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 12:33:16 by jaacosta          #+#    #+#             */
/*   Updated: 2025/07/23 12:33:18 by jaacosta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../../include/minishell.h"

/**
 * Handles backslash escape sequences in a string.
 * Specifically, if a backslash is followed by another backslash,
	a double quote, or a dollar sign,
 * it treats the second character literally. Otherwise,
	it treats the backslash itself literally.
 * The index `*i` is advanced past the handled sequence.
 * @param arg The input string.
 * @param i A pointer to the current index in the string. 
 * @return A newly allocated string containing the escaped 
	character or the backslash itself.
 */
char	*handle_backslash(const char *arg, int *i)
{
	char	*part;

	if (arg[*i + 1] == '\\' || arg[*i + 1] == '"' || arg[*i + 1] == '$')
	{
		part = ft_substr(arg, *i + 1, 1);
		*i += 2;
	}
	else
	{
		part = ft_substr(arg, *i, 1);
		(*i)++;
	}
	return (part);
}

/**
 * Extracts a segment of plain text from a string.
 * It reads characters from the current position until 
	it encounters a dollar sign ($),
 * a double quote ("), a backslash (\), or the null terminator.
 * The index `*i` is advanced past the extracted plain text.
 *
 * @param arg The input string.
 * @param i A pointer to the current index in the string.
 * @return A newly allocated string containing the plain text segment.
 */
char	*handle_plain_segment(const char *arg, int *i)
{
	int	start;

	start = *i;
	while (arg[*i] && arg[*i] != '$' && arg[*i] != '"' && arg[*i] != '\\')
		(*i)++;
	return (ft_substr(arg, start, *i - start));
}

/**
 * Handles the special '$?' variable,
	which represents the exit status of the last command.
 * It increments the index `*i` to skip the '?' character.
 * @param i A pointer to the current index in the string 
	(should be pointing to '?').
 * @param s The current exit status of the last command.
 * @return A newly allocated string containing the integer
	representation of the exit status.
 */
char	*handle_question_mark(int *i, int s)
{
	(*i)++;
	return (ft_itoa(s));
}

/**
 * Extracts a braced variable name (e.g., "${VAR_NAME}") from a string.
 * It advances the index `*i` past the opening brace '{', reads characters
 * until a closing brace '}', and then advances past the closing brace.
 *
 * @param arg The input string.
 * @param i A pointer to the current index in the string
	(should be pointing to '{' after '$').
 * @return A newly allocated string containing the
	variable name (without braces).
 */
char	*extract_braced_var(const char *arg, int *i)
{
	int		start;
	char	*var_name;

	start = ++(*i);
	while (arg[*i] && arg[*i] != '}')
		(*i)++;
	var_name = ft_substr(arg, start, *i - start);
	if (arg[*i] == '}')
		(*i)++;
	return (var_name);
}

/**
 * Extracts a simple variable name (e.g., "$VAR_NAME") from a string.
 * It reads alphanumeric characters and underscores starting
	from the current position.
 * The index `*i` is advanced past the extracted variable name.
 *
 * @param arg The input string.
 * @param i A pointer to the current index in the string (should be
	pointing to the start of the variable name after '$').
 * @return A newly allocated string containing the variable name.
 */
char	*extract_simple_var(const char *arg, int *i)
{
	int	start;

	start = *i;
	while (arg[*i] && (ft_isalnum(arg[*i]) || arg[*i] == '_'))
		(*i)++;
	return (ft_substr(arg, start, *i - start));
}
