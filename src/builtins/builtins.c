/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:05:47 by rmarrero          #+#    #+#             */
/*   Updated: 2025/04/01 13:05:50 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

// --- this functions is internal execve() -- //
void	built(t_command *cmd, char **envp)
{
	int	i;

	envp = envp;
	while (cmd)
	{
		i = 0;
		if (get_builtin_cmd(cmd->args[i]))
		{
			ft_printf("%s", cmd->args[i + 1]);
			execute_builtin(get_builtin_cmd(cmd->args[i]), cmd->args[i + 1]);
			i++;
		}
		cmd = cmd->next;
	}
}

int	get_builtin_cmd(char *cmd)
{
	if (ft_strncmp(cmd, "cd", 2) == 0)
		return (CD);
	if (ft_strncmp(cmd, "exit", 5) == 0)
		return (EXIT);
	if (ft_strncmp(cmd, "echo", 4) == 0)
		return (ECHO);
	if (ft_strncmp(cmd, "pwd", 3) == 0)
		return (PWD);
	if (ft_strncmp(cmd, "export", 5) == 0)
		return (EXPORT);
	if (ft_strncmp(cmd, "unset", 5) == 0)
		return (UNSET);
	if (ft_strncmp(cmd, "clear", 5) == 0)
		return (CLEAR);
	return (-1);
}
// -- design -- //
void	execute_builtin(int cmd, char *path)
{
	if (cmd == CD)
		ft_cd(path);
	else if (cmd == EXIT)
		ft_printf("Ejecutando exit\n");
	else if (cmd == ECHO)
		ft_printf("Ejecutando cd\n");
	else if (cmd == PWD)
		pwd();
	else if (cmd == EXPORT)
		ft_printf("Ejecutando export\n");
	else if (cmd == UNSET)
		ft_printf("Ejecutando unset\n");
	else if (cmd == CLEAR)
		clear();
	else
		ft_printf("not");
}
