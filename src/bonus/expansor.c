/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaacosta <jaacosta@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 16:33:19 by jaacosta          #+#    #+#             */
/*   Updated: 2025/07/14 16:33:21 by jaacosta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/minishell.h"

/**
 * @brief Expands a shell variable within an argument string.
 *
 * This function handles the expansion of environment variables (`$VAR`),
 * special parameters (`$?`), and braced parameters (`${VAR}`) found in
 * an argument string. It identifies the variable name, retrieves its value
 * from the environment, and returns the expanded string.
 *
 * @param arg The full argument string being processed.
 * @param i A pointer to the current index in `arg`. This index will be
 * advanced past the variable name.
 * @param env A pointer to the environment variables list.
 * @param s The exit status of the previous command (used for `$?`).
 * @return A newly allocated string containing the expanded value. Returns a
 * strdup("$") if the '$' is not followed by a valid variable name,
 * an empty string for an unset variable, or NULL on memory allocation failure.
 */
char	*expand_variable_ini(const char *arg, int *i, t_env *env, int s)
{
	char	*var_name;
	char	*value;

	(*i)++;
	if (!arg[*i] || (!ft_isalpha(arg[*i]) && arg[*i] != '_' && arg[*i] != '?'
			&& arg[*i] != '{'))
		return (ft_strdup("$"));
	if (arg[*i] == '?')
		return (handle_question_mark(i, s));
	if (arg[*i] == '{')
		var_name = extract_braced_var(arg, i);
	else
		var_name = extract_simple_var(arg, i);
	if (!var_name)
		return (ft_strdup(""));
	if (ft_strcmp(var_name, "?") == 0)
	{
		free(var_name);
		return (ft_itoa(s));
	}
	value = ft_echo_expand(var_name, env);
	free(var_name);
	if (value)
		return (value);
	return (ft_strdup(""));
}

/**
 * @brief Copies and expands content within double quotes.
 *
 * This function processes a segment of an argument string that is enclosed
 * in double quotes. It handles backslash escapes and variable expansions
 * within these quotes, concatenating the results.
 *
 * @param arg The full argument string.
 * @param i A pointer to the current index in `arg`. This index will be
 * advanced past the end of the double-quoted section.
 * @param env A pointer to the environment variables list.
 * @param s The exit status of the previous command.
 * @return A newly allocated string containing the 
	expanded content within quotes,
 * or NULL on memory allocation failure.
 */
char	*copy_double_quoted_text_ini(const char *arg, int *i, t_env *env, int s)
{
	char	*result;
	char	*part;

	result = ft_calloc(1, sizeof(char));
	if (!result)
		return (NULL);
	(*i)++;
	while (arg[*i] && arg[*i] != '"')
	{
		if (arg[*i] == '\\')
			part = handle_backslash(arg, i);
		else if (arg[*i] == '$')
			part = expand_variable_ini(arg, i, env, s);
		else
			part = handle_plain_segment(arg, i);
		if (!part)
			return (free(result), NULL);
		result = ft_strjoin_free(result, part);
		if (!result)
			return (NULL);
	}
	if (arg[*i] == '"')
		(*i)++;
	return (result);
}

/**
 * @brief Expands an argument string, handling quotes and variable expansions.
 *
 * This is the main expansion function that iterates through an argument string,
 * identifies different segments (plain text, single quotes, double quotes,
 * and variable expansions), processes each segment,
	and concatenates the results
 * to produce the fully expanded string.
 *
 * @param arg The original argument string to be expanded.
 * @param env A pointer to the environment variables list.
 * @param s The exit status of the previous command.
 * @return A newly allocated string representing the expanded argument,
 * or NULL on memory allocation failure.
 */
char	*ft_expand_arg_ini(char *arg, t_env *env, int s)
{
	char	*result;
	char	*part;
	int		i;

	result = ft_calloc(1, sizeof(char));
	part = NULL;
	if (!result)
		return (NULL);
	i = 0;
	while (arg[i])
	{
		part = get_next_part(arg, &i, env, s);
		if (!part)
			return (free(result), NULL);
		if (!append_part(&result, part))
			return (NULL);
	}
	return (result);
}

/**
 * @brief Performs expansions on token values before command execution.
 *
 * This function iterates through a list of tokens. For each token of type
 * `TOKEN_WORD` that contains characters indicating a need for expansion
 * (as determined by `check_expansion`), it calls `ft_expand_arg_ini` to
 * perform the actual expansion. The original token value is then freed
 * and replaced with the expanded value.
 *
 * @param tokens A pointer to the head of the token list. The token values
 * will be modified in place.
 * @param env A pointer to the environment variables list.
 * @param status The exit status of the previous command,
	passed to expansion functions.
 */
void	expand_before_executor(t_token **tokens, t_env *env, int status)
{
	t_token	*cur;
	char	*expanded;

	cur = *tokens;
	expanded = NULL;
	while (cur)
	{
		if (cur->type == TOKEN_WORD && check_expansion(cur->value))
		{
			expanded = ft_expand_arg_ini(cur->value, env, status);
			if (expanded)
			{
				free(cur->value);
				cur->value = expanded;
			}
			else
			{
				free(cur->value);
				cur->value = ft_strdup("");
			}
		}
		cur = cur->next;
	}
}

/**
 * @brief Checks for multiple non-empty expansions at the 
	beginning of a command.
 * This function specifically looks for a scenario where multiple consecutive
 * tokens at the start of a command (before a pipe) result in non-empty
 * expansions. This is often an indication of a syntax error or an unintended
 * expansion that should be flagged (e.g., `echo $VAR1$VAR2` where both expand
 * to non-empty strings,
	but the shell might interpret it as too many arguments).
 *
 * @param tokens A pointer to the head of the token list.
 * @param env A pointer to the environment variables list.
 * @return Returns 1 if multiple non-empty expansions are found at the start,
 * otherwise returns 0.
 */
int	check_multiple_expansions_at_start(t_token *tokens, t_env *env)
{
	t_token	*cur;
	int		non_empty_expansions;

	cur = tokens;
	non_empty_expansions = 0;
	while (cur && cur->type != TOKEN_PIPE)
	{
		if (check_expansion(cur->value))
		{
			if (is_non_empty_expansion(cur->value, env))
			{
				non_empty_expansions++;
				if (non_empty_expansions > 1)
				{
					ft_printf("bash: too many arguments\n");
					return (1);
				}
			}
		}
		else
			break ;
		cur = cur->next;
	}
	return (0);
}
