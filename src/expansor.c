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

char	*copy_plain_text_ini(char *arg, int *i)
{
	int	start;

	start = *i;
	while (arg[*i] && arg[*i] != '$' && arg[*i] != '\'' && arg[*i] != '"'
		&& arg[*i] != '\\')
		(*i)++;
	return (ft_substr(arg, start, *i - start));
}

char	*expand_variable_ini(const char *arg, int *i, t_env *env, int s)
{
	int		start;
	char	*var_name;
	char	*value;

	(*i)++;
	if (!arg[*i] || (!ft_isalpha(arg[*i]) && arg[*i] != '_' && arg[*i] != '?'
			&& arg[*i] != '{'))
		return (ft_strdup("$"));
	if (arg[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(s));
	}
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
	if (!var_name)
		return (ft_strdup(""));
	if (ft_strcmp(var_name, "?") == 0)
	{
		free(var_name);
		return (ft_itoa(s));
	}
	if (var_name)
	{
		value = ft_echo_expand(var_name, env);
		free(var_name);
		if (value)
			return (value);
	}
	return (ft_strdup(""));
}

char	*copy_double_quoted_text_ini(const char *arg, int *i, t_env *env, int s)
{
	char	*result;
	char	*part;
	int		start;

	result = ft_calloc(1, sizeof(char));
	(*i)++;
	while (arg[*i] && arg[*i] != '"')
	{
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
		else if (arg[*i] == '$') // variable expansion
			part = expand_variable_ini(arg, i, env, s);
		else
		{
			start = *i;
			while (arg[*i] && arg[*i] != '$' && arg[*i] != '"'
				&& arg[*i] != '\\')
				(*i)++;
			part = ft_substr(arg, start, *i - start);
		}
		if (!part)
			return (free(result), NULL);
		result = ft_strjoin_free(result, part);
	}
	if (arg[*i] == '"')
		(*i)++;
	return (result);
}

char	*copy_single_quoted_text_ini(const char *arg, int *i)
{
	char	*text;
	int		start;

	start = ++(*i);
	while (arg[*i] && arg[*i] != '\'')
		(*i)++;
	text = ft_substr(arg, start, *i - start);
	if (arg[*i] == '\'')
		(*i)++;
	return (text);
}

char	*ft_expand_arg_ini(char *arg, t_env *env, int s)
{
	char	*result;
	char	*part;
	int		i;

	part = NULL;
	result = ft_calloc(1, sizeof(char));
	i = 0;
	if (!result)
		return (NULL);
	while (arg[i])
	{
		if (arg[i] == '\'')
			part = copy_single_quoted_text_ini(arg, &i);
		else if (arg[i] == '"')
		{
			part = copy_double_quoted_text_ini(arg, &i, env, s);
		}
		else if (arg[i] == '\\' && arg[i + 1] == '$')
		{
			part = ft_substr(arg, i + 1, 1);
			i += 2;
		}
		else if (arg[i] == '$')
			part = expand_variable_ini(arg, &i, env, s);
		else
		{
			part = copy_plain_text_ini(arg, &i);
		}
		if (!part)
			return (free(result), NULL);
		result = ft_strjoin_free(result, part);
	}
	return (result);
}
void	expand_before_executor(t_token **tokens, t_env *env, int status)
{
	t_token	*cur;
	char	*expanded;

	cur = *tokens;
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
	t_token *cur;
	int non_empty_expansions;
	char *expanded;

	cur = tokens;
	non_empty_expansions = 0;
	while (cur && cur->type != TOKEN_PIPE)
	{
		if (check_expansion(cur->value))
		{
			expanded = ft_expand_arg_ini(cur->value, env, 0);
			if (expanded && expanded[0] != '\0')
			{
				non_empty_expansions++;
			}
			free(expanded);
			if (non_empty_expansions > 1)
			{
				ft_printf("bash: too many arguments\n");
				return (1);
			}
		}
		else
			break ;
		cur = cur->next;
	}
	return (0);
}