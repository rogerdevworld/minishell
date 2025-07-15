/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:05:47 by rmarrero          #+#    #+#             */
/*   Updated: 2025/04/01 13:05:50 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/minishell.h"

static char *clean_argument(char *arg)
{
	char *cleaned;
	int i = 0, j = 0;

	if (!arg)
		return NULL;
	cleaned = malloc(ft_strlen(arg) + 1);
	if (!cleaned)
		return NULL;
	while (arg[i])
	{
		if (arg[i] != '"' && arg[i] != '\'')
		{
			cleaned[j] = arg[i];
			j++;
		}
		i++;
	}
	cleaned[j] = '\0';
	return cleaned;
}

/**
 * Elimina comillas simples o dobles alrededor del argumento, si existen.
 */
// static char	*strip_quotes(char *str)
// {
// 	int	len;

// 	if (!str)
// 		return (NULL);
// 	len = ft_strlen(str);
// 	if (len >= 2 && ((str[0] == '"' && str[len - 1] == '"') || (str[0] == '\''
// 				&& str[len - 1] == '\'')))
// 	{
// 		str[len - 1] = '\0';
// 		return (str + 1);
// 	}
// 	return (str);
// }

/**
 * Verifica si la cadena es un número válido (posiblemente con signo).
 */
static int is_numeric(const char *str)
{
	int i;

	i = 0;
	if (!str || !*str)
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

/**
 * Analiza y retorna el código de salida o código de error.
 * Retorna:
 *   -2 si no es numérico
 *   -1 si hay demasiados argumentos
 *    valor numérico si válido
 *    0 si no hay argumento
 */
static int check_exit(char **args)
{
	int count_args = 0;
	char *num;

	while (args[count_args])
		count_args++;
	if (count_args > 2)
		return -1;
	if (count_args == 2)
	{
		num = clean_argument(args[1]); // en lugar de strip_quotes
		if (!is_numeric(num))
		{
			free(num);
			return (-2);
		}
		int value = ft_atoi(num);
		free(num);
		return (value);
	}
	return 0;
}

/**
 * Implementación de la built-in `exit`.
 */
int ft_exit_builtin(char **args)
{
	int status;

	ft_putstr_fd("exit\n", 1); // Como hace bash
	status = check_exit(args);
	if (status == -1)
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		return (1);
	}
	else if (status == -2)
	{
		ft_putstr_fd("exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		exit(2); // Bash devuelve 2 para este error
	}
	else
	{
		exit((unsigned char)status);
	}
	return (status);
}
