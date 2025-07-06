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

/*void	ft_exit_builtin(void)
{
	ft_printf("exit\n");
	exit(0);
}*/
static int	check_exit(char **args)
{
	int	i;
	int	count_args;

	i = 0;
	count_args = 0;
	while (args[count_args])
		count_args++;
	//ft_printf("\n args son: %i\n", count_args);
	if (count_args > 2)
		return (-1);
	if (count_args == 2)
	{
		while (args[1][i])
		{
			if (!ft_isdigit(args[1][i]))
				return (-2);
			i++;
		}
		return (ft_atoi(args[1]));
	}
	return (0);
}

int	ft_exit_builtin(char **args)
{
	int	status;

	status = check_exit(args);
	//ft_putstr_fd("exit\n", 2);
	//ft_printf("exit");
	if (status == -1)
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		
		status = 1;
		//ft_printf("\nel estatus de salida es: %i \n", status);
	}
	else if (status == -2)
	{
		ft_putstr_fd("exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		//exit(2);
		//return (2);
	}
	else
		exit(status);
	return (status);
	//ft_printf("\nel estatus de salida es: %i \n", status);
}

/*
int	ft_exit_builtin(char **args)
{
	int	status;

	status = check_exit(args);
	//ft_putstr_fd("exit\n", 2);
	//ft_printf("exit");
	if (status == -1)
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		
		status = 1;
		//ft_printf("\nel estatus de salida es: %i \n", status);
	}
	else if (status == -2)
	{
		ft_putstr_fd("exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		//exit(2);
		//return (2);
	}
	else
		exit(status);
	//ft_printf("\nel estatus de salida es: %i \n", status);
}
*/
