/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:05:47 by rmarrero          #+#    #+#             */
/*   Updated: 2025/04/09 22:11:07 by xviladri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

void	ft_check_executor(t_command *cmd, char **envp, t_myenv *myenv)
{
	t_executor	ex;
	char		*path;

	ex.prev_fd = -1;
	ex.envp = envp;
	ex.myenv = myenv;
	while (cmd)
	{
		//ft_printf("Before ft_wildcards:\n");
		//print_args(cmd->args);
		ft_wildcards(&(cmd->args));
		//ft_printf("\nAfter ft_wildcards:\n");
		//print_args(cmd->args);
		if (cmd->limiter)
		{
			ft_here_doc(cmd->limiter);
		}
		if (!cmd->args || !cmd->args[0])
		{
			cmd = cmd->next;
			continue ;
		}
		ex.builtin_id = get_builtin_cmd(cmd->args[0]);
		if (ex.builtin_id != -1 && cmd->operator!= PIPE)
		{
			ex.saved_stdin = -1;
			ex.saved_stdout = -1;
			redirections(cmd, &ex.saved_stdin, &ex.saved_stdout);
			execute_builtin(ex.builtin_id, cmd->args, ex.envp, ex.myenv);
			restore_redirections(ex.saved_stdin, ex.saved_stdout);
		}
		else
		{
			// Aquí verificamos si el comando tiene ruta absoluta
			if (cmd->args[0][0] == '/' || cmd->args[0][0] == '.')
			{
				if (access(cmd->args[0], X_OK) != 0)
				{
					ft_printf("minishell: %s: command not found\n",
						cmd->args[0]);
				}
				else
				{
					ex.pid = external_command(cmd, &ex);
					if (cmd->operator!= PIPE)
					{
						waitpid(ex.pid, NULL, 0);
					}
				}
			}
			else
			{
				// Si no tiene ruta, buscamos en el PATH
				path = get_path(cmd->args[0], ex.envp);
				if (path == NULL || access(path, X_OK) != 0)
				{
					ft_printf("minishell: %s: command not found\n",
						cmd->args[0]);
				}
				else
				{
					ex.pid = external_command(cmd, &ex);
					if (cmd->operator!= PIPE)
					{
						waitpid(ex.pid, NULL, 0);
					}
				}
			}
		}
		cmd = cmd->next;
	}
}

// -- function to execute a command -- //
// --  de momento esta funcion esta fuera pero la dejare para comandos unicos --
//
pid_t	external_command(t_command *cmd, t_executor *ex)
{
	if (cmd->operator== PIPE)
	{
		if (pipe(ex->p_fd) == -1)
			ft_exit("pipe failed");
	}
	ex->pid = fork();
	if (ex->pid == -1)
		ft_exit("fork failed");
	if (ex->pid == 0)
		child_process(cmd, ex);
	else
		parent_process(cmd, ex);
	return (ex->pid);
}

// -- hay muchos parametros pero las funciones solo se les pasara dos struct t_shell
// y las struct de las pipes para esta parte aplicare esto mas adelante mini 2.0 --
//
void	child_process(t_command *cmd, t_executor *ex)
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
	if (cmd->operator== PIPE)
	{
		close(ex->p_fd[0]);
		if (cmd->output_file != -1)
			close(ex->p_fd[1]);
		else
		{
			dup2(ex->p_fd[1], STDOUT_FILENO);
			close(ex->p_fd[1]);
		}
	}
	if (cmd->output_file != -1)
	{
		dup2(cmd->output_file, STDOUT_FILENO);
		close(cmd->output_file);
	}
	if (ex->builtin_id != -1)
		execute_builtin(ex->builtin_id, cmd->args, ex->envp, ex->myenv);
	else
		execve(get_path(cmd->args[0], ex->envp), cmd->args, ex->envp);
	exit(0);
}

void	parent_process(t_command *cmd, t_executor *ex)
{
	if (ex->prev_fd != -1)
		close(ex->prev_fd);
	if (cmd->operator== PIPE)
	{
		close(ex->p_fd[1]);
		ex->prev_fd = ex->p_fd[0];
	}
	else
	{
		ex->prev_fd = -1;
	}
}

void	redirections(t_command *cmd, int *saved_stdin, int *saved_stdout)
{
	if (cmd->input_file != -1)
	{
		*saved_stdin = dup(STDIN_FILENO);
		dup2(cmd->input_file, STDIN_FILENO);
		close(cmd->input_file);
	}
	if (cmd->output_file != -1)
	{
		*saved_stdout = dup(STDOUT_FILENO);
		dup2(cmd->output_file, STDOUT_FILENO);
		close(cmd->output_file);
	}
}

void	restore_redirections(int saved_stdin, int saved_stdout)
{
	if (saved_stdin != -1)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
	}
	if (saved_stdout != -1)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
}
#include <stdio.h>

void	print_args(char **args)
{
	int	i;

	i = 0;
	while (args[i] != NULL)
	{
		printf("Argument %d: %s\n", i, args[i]);
		i++;
	}
}
