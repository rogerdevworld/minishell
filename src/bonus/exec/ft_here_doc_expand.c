/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_here_doc_expand.c                                :+:      :+:    :+:  */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:56:13 by rmarrero          #+#    #+#             */
/*   Updated: 2025/03/27 11:57:56 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/**

 * Appends a substring segment to an existing result string 
 	and frees the segment.
 * This is a utility function used during string building,
	particularly in expansion processes.
 * @param result The existing string to which `part` will 
 	be appended. This string will be freed.
 * @param start A pointer to the beginning of the segment in the 
 	source string.
 * @param len The length of the segment to extract.
 * @return A newly allocated string containing the concatenated
 	`result` and `part`.
 * Returns NULL if memory allocation for the new string or substring fails.
 */
static char	*append_str_segment(char *result, const char *start, int len)
{
	char	*part;

	part = ft_substr(start, 0, len);
	return (ft_strjoin_free(result, part));
}

/**
 * Extracts a braced variable name (e.g., "${VAR_NAME}") from a string.
 * This function is designed to be used with a pointer-to-pointer `p`
 * which allows it to advance the reading position in the original string.
 * @param p A pointer to a `const char*` that points to the character 
 	after '$' and at '{'.
 * This pointer will be advanced past the closing '}' of the variable name.
 * @return A newly allocated string containing the variable 
 	name (without braces),
 * or NULL if memory allocation fails.
 */
static char	*extract_braced_var2(const char **p)
{
	const char	*start;
	int			len;

	(*p)++;
	start = *p;
	len = 0;
	while (**p && **p != '}')
	{
		(*p)++;
		len++;
	}
	if (**p == '}')
		(*p)++;
	return (ft_substr(start, 0, len));
}

/**
 * Determines and extracts a variable name from the input string.
 * It handles two formats: braced variables (e.g.,
	"${VAR}") and simple variables (e.g., "$VAR").
 * The `p` pointer is advanced past the extracted variable name.
 *
 * @param p A pointer to a `const char*` that points to the character
 * immediately after a '$' sign (i.e., the start of the variable name).
 * This pointer will be advanced past the entire variable name.
 * @return A newly allocated string containing the variable name,
	or NULL if memory allocation fails.
 */
char	*get_var_name(const char **p)
{
	const char	*start;
	int			len;

	if (**p == '{')
		return (extract_braced_var2(p));
	else
	{
		start = *p;
		len = 0;
		while (**p && (ft_isalnum((unsigned char)**p) || **p == '_'))
		{
			(*p)++;
			len++;
		}
		return (ft_substr(start, 0, len));
	}
}

/**
 * Handles variable expansion logic for a single variable encountered
 	in a string.
 * It specifically handles "$?" (last exit status) and then general 
 	environment variables.
 * If the variable name is not valid or not found,
	it returns an appropriate string.
 * The `p` pointer is advanced past the expanded variable.
 *
 * @param p A pointer to a `const char*` that points to the character
 * immediately after the '$' sign of the variable to be expanded.
 * This pointer will be advanced past the entire variable name.
 * @param status The exit status of the last executed command 
	(for "$?" expansion).
 * @return A newly allocated string containing the expanded 
		value of the variable.
 * Returns "0" for "$?" (exit status), "$" for invalid variable names,
 * the variable's value from the environment,
	or an empty string if the variable is not found.
 * Returns NULL if memory allocation fails.
 */
static char	*handle_expansion(const char **p, int status)
{
	char	*var;
	char	*val;

	if (**p == '?')
	{
		(*p)++;
		return (ft_itoa(status));
	}
	if (!**p || (!ft_isalpha(**p) && **p != '_' && **p != '{'))
		return (ft_strdup("$"));
	var = get_var_name(p);
	if (var)
	{
		val = getenv(var);
		free(var);
		if (val)
			return (ft_strdup(val));
	}
	return (ft_strdup(""));
}

/**
 * Expands environment variables within a given line,
	specifically for heredoc content.
 * It iterates through the input `line`, identifies '$' 
 	characters, and performs
 * variable expansion. It handles plain text segments and appends 
 them to a result string.
 * @param line The input string (e.g., a line read from heredoc) to be expanded.
 * @param status The exit status of the last executed command 
 	(for "$?" expansion).
 * @return A newly allocated string with all environment variables expanded.
 * Returns NULL if memory allocation fails during the process.
 */
char	*expand_line_heredoc(const char *line, int status)
{
	char		*result;
	const char	*p;
	const char	*start;

	result = ft_strdup("");
	p = line;
	while (*p)
	{
		start = p;
		while (*p && *p != '$')
			p++;
		if (p > start)
			result = append_str_segment(result, start, p - start);
		if (*p == '$')
		{
			p++;
			result = ft_strjoin_free(result, handle_expansion(&p, status));
		}
	}
	return (result);
}
