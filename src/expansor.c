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
	while (arg[*i] && arg[*i] != '$' && arg[*i] != '\'' && arg[*i] != '"' && arg[*i] != '\\')
		(*i)++;
	return (ft_substr(arg, start, *i - start));
}

void shift_empty_args(char **args)
{
    int i = 0;
    int j = 0;

    // Avanza mientras haya argumentos vacíos al inicio
    while (args[i] && args[i][0] == '\0')
    {
        free(args[i]); // libera la cadena vacía
        i++;
    }

    // Si hay que desplazar
    if (i > 0)
    {
        // Desplaza todo el resto de argumentos hacia el inicio
        while (args[i])
        {
            args[j++] = args[i++];
        }
        args[j] = NULL; // termina la lista
    }
}


char	*expand_variable_ini(const char *arg, int *i, t_env *env, int s)
{
	int		start;
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
			part = expand_variable_ini(arg, i, env, s);
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

char	*copy_single_quoted_text_ini(const char *arg, int *i)
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

// char	*ft_expand_arg_ini(t_minishell *minishell, char *arg, t_env *env, int s)
// {
// 	char	*result;
// 	char	*part;
// 	int	i;

// 	// ft_printf("\n el arg que entra es \n%s", arg);
// 	part = NULL;
// 	result = ft_calloc(1, sizeof(char));
// 	i = 0;
// 	if (!result)
// 		return (NULL);
// 	while(arg[i])
// 	{
// 		if (arg[i] == '\'')
// 			part = copy_single_quoted_text_ini(arg, &i);//TD--Done
// 		else if (arg[i] == '"')
// 		{
// 			part = copy_double_quoted_text_ini(minishell, arg, &i, env, s);//TD--Done
// 		}
// 		else if (arg[i] == '\\' && arg[i + 1] == '$')
// 		{
// 			part = ft_substr(arg, i + 1, 1);
// 			i += 2;
// 		}
// 		else if (arg[i] == '$')
// 		 	part = expand_variable_ini(minishell, arg, &i, env, s);//TD--Done
// 		else
// 		{
// 			part = copy_plain_text_ini(arg, &i);//TD--Done
// 		}
// 		if (!part)
// 			return (free(result), NULL);
// 		result = ft_strjoin_free(result, part);
// 	}
// 	return (result);
// }

char	*ft_expand_arg_ini(char *arg, t_env *env, int s)
{
	char	*result;
	char	*part;
	int	i;

	// ft_printf("\n el arg que entra es \n%s", arg);
	part = NULL;
	result = ft_calloc(1, sizeof(char));
	i = 0;
	if (!result)
		return (NULL);
	while(arg[i])
	{
		if (arg[i] == '\'')
			part = copy_single_quoted_text_ini(arg, &i);//TD--Done
		else if (arg[i] == '"')
		{
			part = copy_double_quoted_text_ini(arg, &i, env, s);//TD--Done
		}
		else if (arg[i] == '\\' && arg[i + 1] == '$')
		{
			part = ft_substr(arg, i + 1, 1);
			i += 2;
		}
		else if (arg[i] == '$')
		 	part = expand_variable_ini(arg, &i, env, s);//TD--Done
		else
		{
			part = copy_plain_text_ini(arg, &i);//TD--Done
		}
		if (!part)
			return (free(result), NULL);
		result = ft_strjoin_free(result, part);
	}
	return (result);
}
void	expand_before_executor(t_token **tokens, t_env *env, int status)
{
    t_token *cur = *tokens;
    char *expanded;

    while (cur)
    {
        // Si el token es una variable a expandir
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
                // Si la expansión devuelve NULL, reemplaza por cadena vacía
                free(cur->value);
                cur->value = ft_strdup("");
            }
        }
        cur = cur->next;
    }
}