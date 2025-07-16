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

int	msg(char *error, char *arg)
{
	if (!arg)
		ft_putstr_fd(error, 2);
	if (arg)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(error, 2);

	}
	ft_putstr_fd("\n", 2);
	return (1);
}

int	ft_cd(char **path, t_myenv *env)
{
	char	*target_dir;

	target_dir = NULL;
	if (path[2])
		return (msg(" too many arguments", NULL));
	if (!path[1])
	{
		target_dir = ft_echo_expand("HOME", env->list_env);
		// target_dir = ft_getenv("HOME", env->env);
		if (!target_dir)
			return (msg("HOME not set", NULL));
	}
	else if (path[1][0] == '$')
	{
		target_dir = ft_echo_expand(path[1] + 1, env->list_env);
		if (!target_dir)
			return (msg("environment variable not found", path[1]));
	}
	else
		target_dir = path[1];
	if (chdir(target_dir) == -1)
		return (msg("No such file or directory", target_dir));
	return (0);
}
