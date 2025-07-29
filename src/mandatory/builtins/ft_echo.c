/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:05:47 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/15 14:41:58 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../include/minishell.h"

/**
 * Expands a shell variable within a string.
 * Handles '$?' for the last exit status, and other variables from the environment.
 * Increments the index 'i' past the expanded variable.
 * Returns a newly allocated string with the variable's value, or an empty string if not found.
 */
char	*expand_variable(t_minishell *minishell, const char *arg, int *i, int s)
{
	char	*var_name;

	var_name = NULL;
	(*i)++;
	if (!arg[*i] || (!ft_isalpha(arg[*i]) && arg[*i] != '_' && arg[*i] != '?'
			&& arg[*i] != '{'))
		return (ft_strdup("$"));
	if (arg[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(s));
	}
	var_name = extract_variable_name(arg, i);
	if (!var_name)
		return (ft_strdup(""));
	return (get_variable_value(minishell, var_name));
}

/**
 * Copies and expands content within double quotes.
 * Handles variable expansion inside double-quoted strings.
 * Increments the index 'i' past the closing double quote.
 * Returns a newly allocated string with the expanded content.
 */
char	*copy_double_quoted_text(t_minishell *minishell, const char *arg,
		int *i, int s)
{
	char	*result;

	result = NULL;
	(*i)++;
	result = handle_inside_double_quotes(minishell, arg, i, s);
	if (!result)
		return (NULL);
	if (arg[*i] == '"')
		(*i)++;
	return (result);
}

/**
 * Expands all variables and handles quotes within a single argument string.
 * Iterates through the argument, processing segments (plain text, variables, quoted text).
 * Returns a newly allocated string with all expansions applied.
 */
char	*ft_expand_arg(t_minishell *minishell, char *arg, int s)
{
	char	*result;
	char	*part;
	int		i;

	i = 0;
	result = ft_calloc(1, sizeof(char));
	if (!result)
		return (NULL);
	while (arg[i])
	{
		part = process_segment(minishell, arg, &i, s);
		if (!part)
		{
			free(result);
			return (NULL);
		}
		result = join_and_free(result, part);
		if (!result)
			return (NULL);
	}
	return (result);
}

/**
 * Implements the 'echo' built-in command.
 * Processes arguments, handles the '-n' flag for suppressing newlines,
 * and prints the expanded arguments to standard output.
 * Returns 0 on success, or 1 on error during printing.
 */
int	ft_echo(t_minishell *minishsell, char **args, int s)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	while (args[i] && is_valid_n_flag(args[i]))
	{
		newline = 0;
		i++;
	}
	if (print_echo_args(minishsell, args, i, s) != 0)
		return (1);
	if (newline)
		write(1, "\n", 1);
	return (0);
}
