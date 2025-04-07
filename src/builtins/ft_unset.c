/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:05:47 by rmarrero          #+#    #+#             */
/*   Updated: 2025/04/01 13:05:50 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/minishell.h"

void	put_error(char *prefix, char *cmd, char *msg)
{
	ft_printf("%s: %s: %s\n", prefix, cmd, msg);
}

// -- para comprobar q variable q intentamos eliminar no tenga caract. especiales -- //
static int	ft_check_wrong_char(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '=' || s[i] == '?' || s[i] == '!' || s[i] == '.' ||
			s[i] == '+' || s[i] == '}' || s[i] == '{' || s[i] == '-' ||
			s[i] == '\\' || s[i] == '[' || s[i] == ']' || s[i] == '@' ||
			s[i] == '*' || s[i] == '#' || s[i] == '^' || s[i] == '~')
		{
			put_error("bash: unset", s, "not a valid identifier");
			return (1);
		}
		i++;
	}
	return (0);
}

int get_next_quote(int start, char *str, char c)
{
    int i = start;

    while (str[i])
    {
        if (str[i] == c)
            return (i);
        i++;
    }
    return (-1);
 }

static int	ft_unset_strcmp(char *s, char **envp)
{
	int		i;
	char	*var;
	int		len;

	i = 0;
	if (ft_check_wrong_char(s))
		return (1);
	while (envp[i])//recorremos variables entorno y eliminamos la q coincida
	{
		var = envp[i];
		len = get_next_quote(0, var, '=');
		if (ft_strncmp(s, var, len) == 0)//eliminamos y liberamos memory si coincide
		{
			free(envp[i]);
			envp[i] = NULL;//asi se marca como eliminada.
			while (envp[i + 1])//desplazamos todas las variables siguientes para compactar el array
			{
				envp[i] = envp[i + 1];
				i++;
			}
			envp[i] = NULL;
			break ;
		}
		i++;
	}
	return (0);
}

int	ft_unset(char **args, char **envp)
{
	int	i;

	i = 0;
	if (!args || !args[0])
		return (0);
	while (args[i])
	{
		if (ft_unset_strcmp(args[i], envp) == 1)
			return (1);
		i++;
	}
	return (0);
}
