/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:05:47 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/03 14:39:16 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/minishell.h"

// -- print error -- //
int	msg(char *error, char *arg)
{
	ft_putstr_fd(error, 2);
	if (arg)
		ft_putstr_fd(error, 2);
	return (1);
}

// -- cd .. -- //
// -- int chdir(const char *path); -- //
int	ft_cd(char *path, char **envp)
{
	if (!path)
	{
		//chdir(ft_getenv("HOME", envp));
		return (1);
	}
	if (chdir(path) == -1)
	{
		//chdir(ft_getenv("HOME", envp));
		//perror("");
		return (msg("minishell: cd: ", path));
	}
	return (0);
}
