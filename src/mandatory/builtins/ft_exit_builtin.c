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
#include "../../../include/minishell.h"

/**
 * Cleans the argument by removing single and double quotes.
 */
static char	*clean_argument(char *arg)
{
	char	*cleaned;
	int		i;
	int		j;

	i = 0;
	j = 0;
	cleaned = NULL;
	if (!arg)
		return (NULL);
	cleaned = malloc(ft_strlen(arg) + 1);
	if (!cleaned)
		return (NULL);
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
	return (cleaned);
}

/**
 * Checks if the given string is a valid numeric value.
 */
static int	is_numeric(const char *str)
{
	int	i;

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
 * Determines the exit status based on the provided arguments.
 * Returns:
 * -2 if the argument is not numeric.
 * -1 if there are too many arguments.
 * The numeric value if the argument is valid.
 * 0 if no argument is provided.
 */
static int	check_exit(char **args)
{
	int		count_args;
	char	*num;
	int		value;

	count_args = 0;
	while (args[count_args])
		count_args++;
	if (count_args > 2)
		return (-1);
	if (count_args == 2)
	{
		num = clean_argument(args[1]);
		if (!is_numeric(num))
		{
			free(num);
			return (-2);
		}
		value = ft_atoi(num);
		free(num);
		return (value);
	}
	return (0);
}

/**
 * Implements the built-in 'exit' command for the shell.
 */
int	ft_exit_builtin(char **args, t_minishell *minishell)
{
	int		status;
	t_myenv	*myenv_to_free;

	ft_putstr_fd("exit\n", 1);
	status = check_exit(args);
	if (status == -1)
		return (ft_putstr_fd("minishell: exit: too many arguments\n", 2), 1);
	myenv_to_free = minishell->myenv;
	if (status == -2)
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
	}
	free_minishell(minishell);
	free_myenv(myenv_to_free);
	if (status == -2)
		exit(2);
	else
		exit((unsigned char)status);
	return (0);
}
