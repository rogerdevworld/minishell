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
#include "../../include/minishell.h"

void	ft_check_executor(t_command *cmd, char **envp)
{
	int		p_fd[2];
	int		prev_fd;
	pid_t	pid;
	int		builtin_id;

	prev_fd = -1;
	while (cmd)
	{
		if (!cmd->args || !cmd->args[0])
		{
			cmd = cmd->next;
			continue ;
		}
		builtin_id = get_builtin_cmd(cmd->args[0]);
		if (builtin_id != -1 && cmd->operator!= PIPE)
			execute_builtin(builtin_id, cmd->args[1], envp);
		else
			pid = external_command(cmd, envp, builtin_id, &prev_fd,
					p_fd);
		if (cmd->operator!= PIPE)
			waitpid(pid, NULL, 0);
		cmd = cmd->next;
	}
}

// -- function to execute a command -- //
void	ft_exec_cmd(t_command *cmd, char **envp)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		ft_exit("Failed to fork");
	if (pid == 0)
	{
		if (execve(get_path(cmd->args[0], envp), cmd->args, envp) == -1)
			ft_exit("Command execution failed");
		exit(0);
	}
	else if (pid > 0)
		waitpid(pid, NULL, 0);
}

pid_t	external_command(t_command *cmd, char **envp, int builtin_id,
		int *prev_fd, int p_fd[2])
{
	pid_t	pid;

	if (cmd->operator== PIPE)
	{
		if (pipe(p_fd) == -1)
			ft_exit("pipe failed");
	}
	pid = fork();
	if (pid == -1)
		ft_exit("fork failed");
	if (pid == 0)
		child_process(cmd, envp, builtin_id, *prev_fd, p_fd);
	else
		parent_process(cmd, prev_fd, p_fd);
	return (pid);
}

void	child_process(t_command *cmd, char **envp, int builtin_id, int prev_fd,
		int p_fd[2])
{
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (cmd->operator== PIPE)
	{
		close(p_fd[0]);
		dup2(p_fd[1], STDOUT_FILENO);
		close(p_fd[1]);
	}
	if (builtin_id != -1)
		execute_builtin(builtin_id, cmd->args[1], envp);
	else
		execve(get_path(cmd->args[0], envp), cmd->args, envp);
	//ft_exit("command failed");
	exit(0);
}

void	parent_process(t_command *cmd, int *prev_fd, int p_fd[2])
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (cmd->operator== PIPE)
	{
		close(p_fd[1]);
		*prev_fd = p_fd[0];
	}
	else
	{
		*prev_fd = -1;
	}
}
