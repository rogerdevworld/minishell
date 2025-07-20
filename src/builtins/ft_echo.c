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
#include "../include/minishell.h"

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*joined;

	joined = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (joined);
}

char	*ft_echo_expand(char *str, t_env *env)
{
	while (env)
	{
		if (ft_strcmp(str, env->key) == 0)
			return (ft_strdup(env->content));
		env = env->next;
	}
	return (NULL);
}

static int is_valid_n_flag(char *str)
{
    int i;

    if (!str || str[0] != '-')
        return (0);
    i = 1;
    if (str[i] == '\0') // sólo "-"
        return (0);
    while (str[i])
    {
        if (str[i] != 'n')
            return (0);
        i++;
    }
    return (1);
}


char	*copy_plain_text(char *arg, int *i)
{
	int	start;

	start = *i;
	while (arg[*i] && arg[*i] != '$' && arg[*i] != '\'' && arg[*i] != '"' && arg[*i] != '\\')
		(*i)++;
	return (ft_substr(arg, start, *i - start));
}

char	*expand_variable(t_minishell *minishell, const char *arg, int *i, t_env *env, int s)
{
	int	start;
	char	*var_name;
	char	*value;

	// ft_printf("\n al entrar es: %i\n", s);
	(*i)++;
	if (!arg[*i] || (!ft_isalpha(arg[*i]) && arg[*i] != '_' && arg[*i] != '?' && arg[*i] != '{'))
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
		return (ft_itoa(minishell->exit));
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

char	*copy_double_quoted_text(t_minishell *minishell, const char *arg, int *i, t_env *env, int s)
{
	char	*result;
	char	*part;
	int		start;

	result = ft_calloc(1, sizeof(char));
	(*i)++; // skip initial "
	while (arg[*i] && arg[*i] != '"')
	{
		if (arg[*i] == '\\') // escaped $
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
			part = expand_variable(minishell, arg, i, env, s);
		else
		{
			start = *i;
			while (arg[*i] && arg[*i] != '$' && arg[*i] != '"' && arg[*i] != '\\')
				(*i)++;
			part = ft_substr(arg, start, *i - start);
		}
		if (!part)
			return (free(result), NULL);
		result = ft_strjoin_free(result, part);
	}
	if (arg[*i] == '"')
		(*i)++; // skip closing "
	return (result);
}

char	*copy_single_quoted_text(const char *arg, int *i)
{
	char	*text;
	int	start;

	start = ++(*i);
	while (arg[*i] && arg[*i] != '\'')
		(*i)++;
	text = ft_substr(arg, start, *i - start);
	if (arg[*i] == '\'')
		(*i)++;
	return (text);
}

char	*ft_expand_arg(t_minishell *minishell, char *arg, t_env *env, int s)
{
	char	*result;
	char	*part;
	int	i;

	part = NULL;
	result = ft_calloc(1, sizeof(char));
	i = 0;
	if (!result)
		return (NULL);
	while(arg[i])
	{
		if (arg[i] == '\'')
			part = copy_single_quoted_text(arg, &i);//TD--Done
		else if (arg[i] == '"')
		{
			part = copy_double_quoted_text(minishell, arg, &i, env, s);//TD--Done
		}
		else if (arg[i] == '\\' && arg[i + 1] == '$')
		{
			part = ft_substr(arg, i + 1, 1);
			i += 2;
		}
		else if (arg[i] == '\\') // manejar \ seguido de cualquier cosa
		{
			if (arg[i + 1])
			{
				part = ft_substr(arg, i + 1, 1);
				i += 2;
			}
			else
			{
				part = ft_strdup("\\");
				i++;
			}
		}
		else if (arg[i] == '$')
		 	part = expand_variable(minishell, arg, &i, env, s);//TD--Done
		else
		{
			part = copy_plain_text(arg, &i);//TD--Done
		}
		if (!part)
			return (free(result), NULL);
		result = ft_strjoin_free(result, part);
	}
	return (result);
}

int	ft_echo(t_minishell *minishsell, char **args, t_env *env, int s)
{
	int		i;
	int		newline;
	char	*expanded;

	i = 1;
	newline = 1;
	while (args[i] && is_valid_n_flag(args[i]))
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		expanded = ft_expand_arg(minishsell, args[i], env, s);//TD--Done
		if (!expanded)
			return (1);
		ft_printf("%s", expanded);
		free(expanded);
		if (args[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (newline)
		write(1, "\n", 1);
	return (0);
}

/**
 * version antigua de ft_echo
 * con algunos errores a la hora de hacer el expansor
 * y manejar comillas
 */
/* void	ft_echo(char **args, t_env *env)
{
	int		i;
	int		newline;
	char	*expanded;

	i = 1;
	newline = 1;
	if (args[i] && ft_strncmp(args[i], "-n", 3) == 0)
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		if (args[i][0] == '$') // variable a expandir
		{
			expanded = ft_echo_expand(args[i] + 1, env); // quitar el '$'
			if (expanded)
			{
				ft_printf("%s", expanded);
				free(expanded);
			}
		}
		else
			ft_printf("%s", args[i]);
		if (args[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (newline)
		write(1, "\n", 1);
}

char	*ft_echo_expand(char *str, t_env *env)
{
	while (env)
	{
		if (ft_strcmp(str, env->key) == 0)
			return (ft_strdup(env->content));
		env = env->next;
	}
	return (NULL);
} */
