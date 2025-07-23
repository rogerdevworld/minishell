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
#include "../../../include/minishell.h"

int	check_expansion(char *str)
{
	if (!str)
		return (0);
	if (str[0] == '$' && str[1] != '\0')
		return (1);
	if (str[0] == '"' && str[1] == '$')
		return (1);
	return (0);
}

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
