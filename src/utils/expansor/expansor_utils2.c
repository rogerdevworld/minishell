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
#include "../../../include/minishell.h"

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

char	*handle_plain_segment(const char *arg, int *i)
{
	int	start;

	start = *i;
	while (arg[*i] && arg[*i] != '$' && arg[*i] != '"' && arg[*i] != '\\')
		(*i)++;
	return (ft_substr(arg, start, *i - start));
}

char	*handle_question_mark(int *i, int s)
{
	(*i)++;
	return (ft_itoa(s));
}

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

char	*extract_simple_var(const char *arg, int *i)
{
	int	start;

	start = *i;
	while (arg[*i] && (ft_isalnum(arg[*i]) || arg[*i] == '_'))
		(*i)++;
	return (ft_substr(arg, start, *i - start));
}
