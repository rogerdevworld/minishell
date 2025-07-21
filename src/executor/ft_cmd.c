/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 20:20:30 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/16 20:20:38 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

	// comando a probar
	// > file echo "hola" << start > file1 | echo "adios" << end  > file2
int	execute_command(t_command *cmd, char **envp, t_myenv *myenv,
	t_minishell *minishell, int status)
{
	int		builtin_id;
	pid_t	pid;
	char	**clean_args;	

	builtin_id = -1;
	//if (!cmd || !cmd->args || !cmd->args[0])
	//	return (1);
	status = update_exit_status(status, minishell);
	if (cmd && cmd->args && cmd->args[0])  
	{
		ft_wildcards(&(cmd->args));
		builtin_id = get_builtin_cmd(cmd->args[0]);
	}
	if (builtin_id == -1)
	{
		clean_args = remove_quotes_from_args(cmd->args);
		cmd->args = clean_args;
		g_signal = S_CMD;
		pid = fork();
		if (pid == 0)
		{
			set_defaul_signals();
			if (cmd->redir)
			{
				if (ft_output_redirections(cmd->redir) == -1)
					exit(1);
				if (ft_input_redirection(cmd->redir) == -1)
					exit(1);
				if (cmd->redir->input_file != -1)
					dup2(cmd->redir->input_file, STDIN_FILENO);
				if (cmd->redir->output_file != -1)
					dup2(cmd->redir->output_file, STDOUT_FILENO);
			}
			resolve_command_path(cmd, envp);
			execve(cmd->path, cmd->args, envp);
			if (cmd->redir->output_file !=0)
				exit(0);
			msg("command not found", cmd->args[0]);
			exit(1);
		}
		waitpid(pid, &status, 0);
		status = update_exit_status(status, minishell);
		return (status);
	}
	else
	{
		// 🔹 BUILTINS
		minishell->executor->builtin_id = builtin_id;
		if (!cmd->redir || builtin_id == 0 || builtin_id == 4 || builtin_id == 5
			|| builtin_id == 1
			|| (cmd->redir->input_file == -1 && cmd->redir->output_file == -1))
			return (execute_builtin(minishell, cmd->args, myenv, status));
		else
		{
			// necesita fork por redirecciones
			g_signal = S_CMD;
			pid = fork();
			if (pid == 0)
			{
				set_defaul_signals();
				if (ft_output_redirections(cmd->redir) == -1)
					exit(1);
				if (ft_input_redirection(cmd->redir) == -1)
					exit(1);
				if (cmd->redir->input_file != -1)
					dup2(cmd->redir->input_file, STDIN_FILENO);
				if (cmd->redir->output_file != -1)
					dup2(cmd->redir->output_file, STDOUT_FILENO);
				exit(execute_builtin(minishell, cmd->args, myenv, status));
			}
			waitpid(pid, &status, 0);
			status = update_exit_status(status, minishell);
			return (status);
		}
	}
}


/* int	execute_command(t_command *cmd, char **envp, t_myenv *myenv,
	t_minishell *minishell, int status)
{
	int		builtin_id;
	pid_t	pid;
	char	**clean_args;

	// comando a probar
	// > file echo "hola" << start > file1 | echo "adios" << end  >> file2
	if (!cmd || !cmd->args)
		return (1);
	// Procesa todos los heredocs en el padre antes de fork/exec
	if (cmd->redir && cmd->redir->heredoc_count > 0)
	{
		g_signal = S_HEREDOC;
		if (process_all_heredocs(cmd->redir) == -1)
		{
			// interrumpido por Ctrl+C
			g_signal = S_BASE;
			return (130);
		}
		g_signal = S_BASE;
	}
	status = update_exit_status(status, minishell);
	if (!cmd->args[0])
		return (1);
	builtin_id = get_builtin_cmd(cmd->args[0]);
	// 🔹 COMANDOS EXTERNOS
	ft_wildcards(&(cmd->args));
	if (builtin_id == -1)
	{
		// Limpia comillas
		clean_args = remove_quotes_from_args(cmd->args);
		cmd->args = clean_args;
		g_signal = S_CMD;
		pid = fork();
		if (pid == 0)
		{
			set_defaul_signals();
			// aplica redirecciones (heredoc ya procesado en padre)
			if (cmd->redir)
			{
				// ft_printf("\n output file fd = %d\n", cmd->redir->output_file);
				if (ft_output_redirections(cmd->redir) == -1)
					exit(1);
				if (ft_input_redirection(cmd->redir) == -1)
					exit(1);
				if (cmd->redir->input_file != -1)
					dup2(cmd->redir->input_file, STDIN_FILENO);
				if (cmd->redir->output_file != -1)
					dup2(cmd->redir->output_file, STDOUT_FILENO);
			}
			resolve_command_path(cmd, envp);
			execve(cmd->path, cmd->args, envp);
			perror("execve");
			free_split(clean_args);
			exit(1);
		}
		waitpid(pid, &status, 0);
		status = update_exit_status(status, minishell);
		return (status);
	}
	else// 🔹 BUILTINS
	{
		minishell->executor->builtin_id = builtin_id;
		// builtins sin necesidad de fork (cd, export, unset, etc.)
		if (!cmd->redir || builtin_id == 0 || builtin_id == 4 || builtin_id == 5
			|| (cmd->redir->input_file == -1 && cmd->redir->output_file == -1))
			return (execute_builtin(minishell, cmd->args, myenv, status));
		else
		{
			// builtins que requieren fork por redirecciones
			g_signal = S_CMD;
			pid = fork();
			if (pid == 0)
			{
				set_defaul_signals();
				if (ft_output_redirections(cmd->redir) == -1)
					exit(1);
				if (ft_input_redirection(cmd->redir) == -1)
					exit(1);
				if (cmd->redir->input_file != -1)
					dup2(cmd->redir->input_file, STDIN_FILENO);
				if (cmd->redir->output_file != -1)
					dup2(cmd->redir->output_file, STDOUT_FILENO);
				exit(execute_builtin(minishell, cmd->args, myenv, status));
			}
			waitpid(pid, &status, 0);
			status = update_exit_status(status, minishell);
			return (status);
		}
	}
}
 */

/* int	execute_command(t_command *cmd, char **envp, t_myenv *myenv,
		t_minishell *minishell, int status)
{
	int		builtin_id;
	pid_t	pid;
	char	**clean_args;

	// if (!cmd || !cmd->args || !cmd->args[0])
	// 	return (1);
	ft_wildcards(&(cmd->args));
	builtin_id = get_builtin_cmd(cmd->args[0]);
	if (builtin_id == -1)
	{
		clean_args = remove_quotes_from_args(cmd->args);
		cmd->args = clean_args;
		g_signal = S_CMD;
		pid = fork();
		if (pid == 0)
		{
			set_defaul_signals();
			if (cmd->redir)
			{
				if (ft_output_redirections(cmd->redir) == -1)
					exit (1);
				if (ft_input_redirection(cmd->redir) == -1)
					exit (1);
				if (cmd->redir->input_file != -1)
					dup2(cmd->redir->input_file, STDIN_FILENO);
				if (cmd->redir->output_file != -1)
					dup2(cmd->redir->output_file, STDOUT_FILENO);
			}
			resolve_command_path(cmd, envp);
			execve(cmd->path, cmd->args, envp);
			perror("execve");
			free_split(clean_args);
			exit(1);
		}
		waitpid(pid, &status, 0);
		status = update_exit_status(status, minishell);
		return (status);
	}
	else // Es builtin
	{
		minishell->executor->builtin_id = builtin_id;
		if (!cmd->redir || builtin_id == 0 || builtin_id == 4 || builtin_id == 5
			|| (cmd->redir->input_file == -1 && cmd->redir->output_file == -1))
			// hay que agregar todos los que no son de impresion como export y cd
			return (execute_builtin(minishell, cmd->args, myenv, status));
		else
		{
			g_signal = S_CMD;
			pid = fork();
			if (pid == 0)
			{
				set_defaul_signals();
				if (ft_output_redirections(cmd->redir) == -1)
					exit(1);
				if (ft_input_redirection(cmd->redir) == -1)
					exit(1);
				if (cmd->redir->input_file != -1)
					dup2(cmd->redir->input_file, STDIN_FILENO);
				if (cmd->redir->output_file != -1)
					dup2(cmd->redir->output_file, STDOUT_FILENO);
				exit(execute_builtin(minishell, cmd->args, myenv, status));
			}
			waitpid(pid, &status, 0);
			status = update_exit_status(status, minishell);
			return (status);
		}
	}
}
 */