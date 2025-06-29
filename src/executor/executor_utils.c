/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xviladri <xviladri@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 19:13:03 by xviladri          #+#    #+#             */
/*   Updated: 2025/06/29 19:13:05 by xviladri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

int	handle_builtin(t_command *cmd, t_executor *ex, char **envp, t_myenv *myenv)
{
	if (ex->builtin_id != -1 && cmd->operator != PIPE && ex->prev_fd == -1)
	{
		execute_builtin_command(cmd, ex, envp, myenv);
		return (1);
	}
	return (0);
}

pid_t	external_command(t_command *cmd, t_executor *ex)
{
	int	pipefd[2];

	if (cmd->operator == PIPE)
	{
		if (pipe(ex->p_fd) == -1)
			ft_exit("pipe failed");
	}
	ex->pid = fork();
	if (ex->pid == -1)
		ft_exit("fork failed");
	if (ex->pid == 0)
		child_process(cmd, ex, pipefd);
	else
		parent_process(cmd, ex);
	return (ex->pid);
}

void	child_process(t_command *cmd, t_executor *ex, int pipefd[2])
{
	if (cmd->input_file != -1)
	{
		dup2(cmd->input_file, STDIN_FILENO);
		close(cmd->input_file);
	}
	else if (ex->prev_fd != -1)
	{
		dup2(ex->prev_fd, STDIN_FILENO);
		close(ex->prev_fd);
	}
	if (cmd->output_file != -1)
	{
		dup2(cmd->output_file, STDOUT_FILENO);
		close(cmd->output_file);
	}
	else if (cmd->operator == PIPE)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
	}
	execute_external_command(cmd, ex);
}

void	execute_external_command(t_command *cmd, t_executor *ex)
{
	char	*path;

	path = get_path(cmd->args[0], ex->envp);
	if (!path || access(path, X_OK) != 0)
	{
		write(2, "minishell: command not found\n", 30);
		exit(127);
	}
	execve(path, cmd->args, ex->myenv->env);
	perror("execve");
	exit(127);
}

void	wait_for_children(pid_t *pids, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		waitpid(pids[i], NULL, 0);
		i++;
	}
}
