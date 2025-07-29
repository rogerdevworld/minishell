/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaacosta <jaacosta@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 13:57:15 by jaacosta          #+#    #+#             */
/*   Updated: 2025/07/23 13:57:20 by jaacosta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../include/minishell.h"

int	built_env(t_myenv *myenv, char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i], "env") != 0)
		{
			ft_putstr_fd("env: '", 2);
			ft_putstr_fd(args[i], 2);
			ft_putendl_fd("': No such file or directory", 2);
			return (127);
		}
		i++;
	}
	print_env(myenv);
	return (0);
}
