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

static char	*handle_quotes(t_minishell *minishell, char *arg, int *i, int s)
{
	if (arg[*i] == '\'')
		return (copy_single_quoted_text(arg, i));
	else if (arg[*i] == '"')
		return (copy_double_quoted_text(minishell, arg, i, s));
	return (NULL);
}

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

char	*join_and_free(char *result, char *part)
{
	char	*joined;

	joined = ft_strjoin_free(result, part);
	return (joined);
}

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
