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
