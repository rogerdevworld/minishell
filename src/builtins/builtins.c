/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:05:47 by rmarrero          #+#    #+#             */
/*   Updated: 2025/04/09 21:37:24 by xviladri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

int	get_builtin_cmd(char *cmd)
{
	char	*clean_cmd;

	clean_cmd = remove_all_quotes(cmd);
	if (ft_strncmp(clean_cmd, "cd", 2) == 0 && ft_strlen(clean_cmd) == 2)
		return (CD);
	if (ft_strncmp(clean_cmd, "exit", 4) == 0 && ft_strlen(clean_cmd) == 4)
		return (EXIT);
	if (ft_strncmp(clean_cmd, "echo", 4) == 0 && ft_strlen(clean_cmd) == 4)
		return (ECHO);
	if (ft_strncmp(clean_cmd, "pwd", 3) == 0 && ft_strlen(clean_cmd) == 3)
		return (PWD);
	if (ft_strncmp(clean_cmd, "export", 6) == 0 && ft_strlen(clean_cmd) == 6)
		return (EXPORT);
	if (ft_strncmp(clean_cmd, "unset", 5) == 0 && ft_strlen(clean_cmd) == 5)
		return (UNSET);
	if (ft_strncmp(clean_cmd, "env", 3) == 0 && ft_strlen(clean_cmd) == 3)
		return (ENV);
	return (-1);
}

int	execute_builtin(t_minishell *minishell, char **args, int s, int builtin_id)
{
	int	status;

	status = 0;
	if (builtin_id == CD)
		status = ft_cd(args, minishell->myenv);
	if (builtin_id == EXIT)
		status = ft_exit_builtin(args);
	if (builtin_id == ECHO)
		status = ft_echo(minishell, args, s);
	if (builtin_id == PWD)
		status = pwd();
	if (builtin_id == EXPORT)
		status = ft_export(args, minishell->myenv);
	if (builtin_id == UNSET)
		ft_unset(args, minishell->myenv);
	if (builtin_id == ENV)
		print_env(minishell->myenv);
	if (builtin_id == CLEAR)
		clear();
	return (status);
}
