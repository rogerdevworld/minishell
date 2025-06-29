/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:05:47 by rmarrero          #+#    #+#             */
/*   Updated: 2025/06/29 18:59:04 by xviladri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

void	parent_process(t_command *cmd, t_executor *ex)
{
	if (ex->prev_fd != -1)
		close(ex->prev_fd);
	if (cmd->operator == PIPE)
	{
		close(ex->p_fd[1]);
		ex->prev_fd = ex->p_fd[0];
	}
	else
		ex->prev_fd = -1;
}

void	execute_command(t_command *cmd, t_executor *ex, pid_t *pids, int *i)
{
	if (cmd->limiter)
		ft_here_doc(cmd->limiter);
	if (!cmd->args || !cmd->args[0])
		return ;
	ex->builtin_id = get_builtin_cmd(cmd->args[0]);
	if (!handle_builtin(cmd, ex, ex->envp, ex->myenv))
	{
		ex->pid = external_command(cmd, ex);
		if (ex->pid > 0)
			pids[(*i)++] = ex->pid;
	}
}

void	ft_check_executor(t_command *cmd, char **envp, t_myenv *myenv)
{
	t_executor	ex;
	pid_t		pids[1024];
	int			i;

	i = 0;
	ex.prev_fd = -1;
	ex.envp = envp;
	ex.myenv = myenv;
	while (cmd)
	{
		execute_command(cmd, &ex, pids, &i);
		cmd = cmd->next;
	}
	while (i--)
		waitpid(pids[i], NULL, 0);
}

void	execute_builtin_command(t_command *cmd, t_executor *ex, char **envp,
		t_myenv *myenv)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (cmd->input_file != -1)
	{
		dup2(cmd->input_file, STDIN_FILENO);
		close(cmd->input_file);
	}
	if (cmd->output_file != -1)
	{
		dup2(cmd->output_file, STDOUT_FILENO);
		close(cmd->output_file);
	}
	execute_builtin(ex->builtin_id, cmd->args, envp, myenv);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}
