/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:05:47 by rmarrero          #+#    #+#             */
/*   Updated: 2025/04/01 13:05:50 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/minishell.h"

void	ft_check_executor(t_command *cmd, t_token *token, char **envp)
{
	int	i;

	i = 0;
	while (cmd)
	{
		if (get_builtin_cmd(cmd->args[i]) != -1)
		{
			execute_builtin(get_builtin_cmd(cmd->args[i]), token);
			i++;
		}
		else
            ft_exec_cmd(cmd, envp);
		cmd = cmd->next;
	}
}

// -- function to execute a command -- //
void	ft_exec_cmd(t_command *cmd, char **envp)
{
	pid_t pid = fork();


	if (pid == 0)
	{
		if (execve(get_path(cmd->args[0], envp), cmd->args, envp) == -1)
			ft_exit("Command execution failed");
		exit(0);
	}
	else if (pid > 0)
		waitpid(pid, NULL, 0);

}