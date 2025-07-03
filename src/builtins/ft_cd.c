/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:05:47 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/03 14:59:56 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/minishell.h"

// -- print error -- //
int	msg(char *error, char *arg)
{
	ft_putstr_fd(error, 2);
	if (arg)
	{
		ft_putstr_fd(" ", 2);
		ft_putstr_fd(error, 2);
	}
	ft_putstr_fd("\n", 2);
	return (1);
}

// -- cd .. -- //
// -- int chdir(const char *path); -- //
int	ft_cd(char *path, char **envp)
{
	if (!path)
	{
		//chdir(ft_getenv("HOME", envp));
		return (msg("cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory", NULL));
	}
	if (chdir(path) == -1)
	{
		//chdir(ft_getenv("HOME", envp));
		//perror("");
		return (msg("minishell: cd: ", path));
	}
	return (0);
}
