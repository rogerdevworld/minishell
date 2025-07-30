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
#include "../../../include/minishell.h"

/**
 * Handles input and output redirections for a command.
 * It determines the correct order of redirections based on 
 	`input_ord` and `output_ord`.
 * Opens specified input/output files and redirects STDIN/STDOUT accordingly.
 * Exits with status 1 on any redirection error.
 */
void	ft_redirect(t_command *cmd)
{
	if (cmd->redir->input_ord > cmd->redir->output_ord
		|| cmd->redir->output_ord > 100)
	{
		if (ft_output_redirections(cmd->redir) == -1)
			exit(1);
		if (ft_input_redirection(cmd->redir) == -1)
			exit(1);
	}
	else
	{
		if (ft_input_redirection(cmd->redir) == -1)
			exit(1);
		if (ft_output_redirections(cmd->redir) == -1)
			exit(1);
	}
	if (cmd->redir->input_file != -1)
		dup2(cmd->redir->input_file, STDIN_FILENO);
	if (cmd->redir->output_file != -1)
		dup2(cmd->redir->output_file, STDOUT_FILENO);
}

/**
 * Validates the command name and determines if it's a built-in command.
 * Checks for internal whitespace in the command name,
	treating it as "command not found" if present.
 * Applies wildcard expansion to command arguments.
 * Returns the ID of the built-in command, -1 if it's an external command, or
	-2 for an invalid command name.
 */
int	validate_and_get_builtin(t_command *cmd)
{
	if (has_internal_whitespace(cmd->args[0]))
	{
		ft_putstr_fd(cmd->args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		return (-2);
	}
	if (cmd && cmd->args && cmd->args[0])
	{
		ft_wildcards(&(cmd->args));
		return (get_builtin_cmd(cmd->args[0]));
	}
	return (-1);
}

/**
 * Executes an external (non-built-in) command.
 * Forks a new process, handles redirections in the child,
	resolves the command path,
 * and executes the command using `execve`.
 * Waits for the child process to complete and updates the shell's exit status.
 * Returns the exit status of the executed command.
 */
int	execute_external_command(t_command *cmd, t_minishell *minishell)
{
	pid_t	pid;
	int		status;
	char	**clean_args;

	clean_args = remove_quotes_from_args(cmd->args);
	free_split(cmd->args);
	cmd->args = clean_args;
	g_signal = S_CMD;
	pid = fork();
	if (pid == 0)
	{
		set_defaul_signals();
		if (cmd->redir)
			ft_redirect(cmd);
		resolve_command_path(cmd, minishell->myenv->env);
		execve(cmd->path, cmd->args, minishell->myenv->env);
		if (cmd->redir && cmd->redir->output_file != 0)
			exit(0);
		msg("command not found", cmd->args[0]);
		exit(1);
	}
	waitpid(pid, &status, 0);
	return (update_exit_status(status, minishell));
}

/**
 * Executes an internal (built-in) command.
 * For certain built-ins (cd, exit, export, unset, env),
	they are executed directly in the parent process.
 * For other built-ins or if redirections are involved,
	a child process is forked to execute the built-in,
 * handling redirections within that child process.
 * Returns the exit status of the executed built-in command.
 */
int	execute_internal_command(t_minishell *minishell, t_command *cmd, int status,
		int builtin_id)
{
	pid_t	pid;

	if (!cmd->redir || builtin_id == 0 || builtin_id == 4 || builtin_id == 5
		|| builtin_id == 1 || (cmd->redir->input_file == -1
			&& cmd->redir->output_file == -1))
	{
		return (execute_builtin(minishell, cmd->args, status, builtin_id));
	}
	else
	{
		g_signal = S_CMD;
		pid = fork();
		if (pid == 0)
		{
			set_defaul_signals();
			if (cmd->redir)
				ft_redirect(cmd);
			exit(execute_builtin(minishell, cmd->args, status, builtin_id));
		}
		waitpid(pid, &status, 0);
		return (update_exit_status(status, minishell));
	}
}

/**
 * Main function to execute a command.
 * It first determines if the command is a built-in or an external command.
 * Based on the type, it calls the appropriate execution function.
 * Returns the exit status of the executed command.
 */
int	execute_command(t_command *cmd, t_minishell *minishell, int status)
{
	int	builtin_id;

	builtin_id = validate_and_get_builtin(cmd);
	if (builtin_id == -2)
		return (EXIT_CMD_NOT_FOUND);
	if (builtin_id == -1)
		return (execute_external_command(cmd, minishell));
	else
		return (execute_internal_command(minishell, cmd, status, builtin_id));
}
