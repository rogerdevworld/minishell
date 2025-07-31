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
#include "../../../include/minishell.h"

/**
 * Prints an error message to standard error.
 * If 'arg' is provided, it prints 'arg', then ": ", then 'error'.
 * If 'arg' is NULL, it just prints 'error'.
 * Returns 1 (typically indicating an error).
 */
int	msg(char *error, char *arg)
{
	if (!arg)
		ft_putstr_fd(error, 2);
	if (arg)
	{
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(error, 2);
	}
	ft_putstr_fd("\n", 2);
	return (1);
}

/**
 * Implements the 'cd' built-in command.
 * Changes the current directory. Handles cases for no arguments (HOME),
 * environment variables, and specific paths.
 * Returns 0 on success, or 1 on error.
 */
void	should_free_target(int should_free, char *target_dir)
{
	if (should_free)
		free(target_dir);
}

static char	*get_target_dir(char **path, t_myenv *env, int *should_free)
{
	char	*target_dir;

	if (!path[1])
	{
		target_dir = ft_echo_expand("HOME", env->list_env);
		if (!target_dir)
			return (msg("HOME not set", NULL), NULL);
		*should_free = 1;
	}
	else if (path[1][0] == '$')
	{
		target_dir = ft_echo_expand(path[1] + 1, env->list_env);
		if (!target_dir)
			return (msg("environment variable not found", path[1]), NULL);
		*should_free = 1;
	}
	else
	{
		target_dir = path[1];
		*should_free = 0;
	}
	return (target_dir);
}

int	ft_cd(char **path, t_myenv *env)
{
	char	*target_dir;
	int		should_free;

	if (path[1] && path[2])
		return (msg(" too many arguments", NULL));
	target_dir = get_target_dir(path, env, &should_free);
	if (!target_dir)
		return (1);
	if (chdir(target_dir) == -1)
	{
		should_free_target(should_free, target_dir);
		return (msg("No such file or directory", target_dir));
	}
	should_free_target(should_free, target_dir);
	return (0);
}
