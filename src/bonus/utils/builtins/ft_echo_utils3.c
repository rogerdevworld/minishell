/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo_utils3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaacosta <jaacosta@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 23:53:17 by jaacosta          #+#    #+#             */
/*   Updated: 2025/07/21 23:53:19 by jaacosta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../../include/minishell.h"

char	*extract_variable_name(const char *arg, int *i)
{
	int		start;
	char	*var_name;

	var_name = NULL;
	if (arg[*i] == '{')
	{
		start = ++(*i);
		while (arg[*i] && arg[*i] != '}')
			(*i)++;
		var_name = ft_substr(arg, start, *i - start);
		if (arg[*i] == '}')
			(*i)++;
	}
	else
	{
		start = *i;
		while (arg[*i] && (ft_isalnum(arg[*i]) || arg[*i] == '_'))
			(*i)++;
		var_name = ft_substr(arg, start, *i - start);
	}
	return (var_name);
}

char	*get_variable_value(t_minishell *minishell, char *var_name)
{
	char	*value;

	value = NULL;
	if (ft_strcmp(var_name, "?") == 0)
	{
		free(var_name);
		return (ft_itoa(minishell->exit));
	}
	value = ft_echo_expand(var_name, minishell->myenv->list_env);
	free(var_name);
	if (value)
		return (value);
	return (ft_strdup(""));
}

static char	*handle_backslash_or_plain(const char *arg, int *i)
{
	char	*part;
	int		start;

	part = NULL;
	if (arg[*i] == '\\')
	{
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
	}
	else
	{
		start = *i;
		while (arg[*i] && arg[*i] != '$' && arg[*i] != '"' && arg[*i] != '\\')
			(*i)++;
		part = ft_substr(arg, start, *i - start);
	}
	return (part);
}

static char	*process_double_quote_segment(t_minishell *minishell,
		const char *arg, int *i, int s)
{
	char	*part;

	part = NULL;
	if (arg[*i] == '$')
		part = expand_variable(minishell, arg, i, s);
	else
		part = handle_backslash_or_plain(arg, i);
	return (part);
}

char	*handle_inside_double_quotes(t_minishell *minishell, const char *arg,
		int *i, int s)
{
	char	*result;
	char	*part;

	result = ft_calloc(1, sizeof(char));
	part = NULL;
	if (!result)
		return (NULL);
	while (arg[*i] && arg[*i] != '"')
	{
		part = process_double_quote_segment(minishell, arg, i, s);
		if (!part)
		{
			free(result);
			return (NULL);
		}
		result = ft_strjoin_free(result, part);
	}
	return (result);
}
