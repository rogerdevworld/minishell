/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaacosta <jaacosta@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 11:40:34 by jaacosta          #+#    #+#             */
/*   Updated: 2025/07/23 11:40:37 by jaacosta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../../include/minishell.h"

/**
 * Checks if a string likely contains an expansion that needs to be performed.
 * An expansion is indicated by a string starting with '$' 
	(and not just '$' itself)
 * or a string starting with '"$' (double quote followed by dollar sign).
 *
 * @param str The string to check for expansion.
 * @return 1 if the string indicates an expansion, 0 otherwise.
 */
int	check_expansion(char *str)
{
	int		in_single;
	int		in_double;
	int		i;

	in_single = 0;
	in_double = 0;
	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (str[i] == '"' && !in_single)
			in_double = !in_double;
		else if (str[i] == '$' && !in_single && str[i + 1] != '\0')
			return (1);
		i++;
	}
	return (0);
}

/**
 * Checks if a given value, after potential expansion,
	results in a non-empty string.
 * It uses `check_expansion` to determine if expansion is needed.
 * If so, it performs a preliminary expansion using `ft_expand_arg_ini`
 * and checks if the result is non-NULL and not an empty string.
 *
 * @param value The string value to check for non-empty expansion.
 * @param env A pointer to the head of the environment variables linked list.
 * @return 1 if the expanded value is non-empty,
	0 otherwise (including no expansion or empty expansion).
 */
int	is_non_empty_expansion(char *value, t_env *env)
{
	char	*expanded;
	int		result;

	result = 0;
	expanded = NULL;
	if (check_expansion(value))
	{
		expanded = ft_expand_arg_ini(value, env, 0);
		if (expanded && expanded[0] != '\0')
			result = 1;
		free(expanded);
	}
	return (result);
}

/**
 * Determines and extracts the next segment (part) from an argument 
	string for expansion.
 * This function acts as a dispatcher based on the current character:
 * - If it's a single quote ('),
	it extracts content within single quotes (no expansion inside).
 * - If it's a double quote ("),
	it extracts content within double quotes (with expansion inside).
 * - If it's a backslash followed by a dollar sign (\$),
	it treats the dollar sign literally.
 * - If it's a dollar sign ($), it triggers variable expansion.
 * - Otherwise,
	it copies a segment of plain text until a special character is met.
 *
 * The index `i` is advanced past the extracted part.
 *
 * @param arg The full argument string being processed.
 * @param i A pointer to the current index within the `arg` string.
 * @param env A pointer to the head of the environment variables linked list.
 * @param s The current exit status of the last command (for `$?` expansion).
 * @return A newly allocated string containing the next processed part,
	or NULL on allocation failure.
 */
char	*get_next_part(char *arg, int *i, t_env *env, int s)
{
	char	*tmp;

	if (arg[*i] == '\'')
		return (copy_single_quoted_text(arg, i));
	else if (arg[*i] == '"')
		return (copy_double_quoted_text_ini(arg, i, env, s));
	else if (arg[*i] == '\\' && arg[*i + 1] == '$')
	{
		tmp = ft_substr(arg, *i + 1, 1);
		*i += 2;
		return (tmp);
	}
	else if (arg[*i] == '$')
		return (expand_variable_ini(arg, i, env, s));
	else
		return (copy_plain_text(arg, i));
}

/**
 * Appends a new `part` to the `result` string and handles memory management.
 * It performs a string join,
	freeing the original `result` string in the process.
 *
 * @param result A pointer to the string accumulating the expanded parts.
 * This string will be freed and updated to the new joined string.
 * @param part The new part string to append. This string will also be freed.
 * @return 1 on successful append, 0 on memory allocation failure during join.
 */
int	append_part(char **result, char *part)
{
	char	*tmp;

	tmp = ft_strjoin_free(*result, part);
	if (!tmp)
	{
		free(*result);
		return (0);
	}
	*result = tmp;
	return (1);
}
