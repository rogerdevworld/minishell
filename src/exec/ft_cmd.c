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

int has_internal_whitespace(const char *str)
	{
		int i = 0;
		if (!str)
			return 0;
		while (str[i])
		{
			if (ft_isspace((unsigned char)str[i]))
				return 1;
			i++;
		}
		return 0;
	}


int	execute_command(t_command *cmd, char **envp,
		t_minishell *minishell, int status)
{
	int		builtin_id;
	pid_t	pid;
	char	**clean_args;

	builtin_id = -1;
	if (has_internal_whitespace(cmd->args[0]))
	{
		ft_putstr_fd(cmd->args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		status = 127;
		return (status);
	}
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
				ft_redirect(cmd);
			resolve_command_path(cmd, envp);
			execve(cmd->path, cmd->args, envp);
			if (cmd->redir->output_file != 0)
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
		if (!cmd->redir || builtin_id == 0 || builtin_id == 4 || builtin_id == 5
			|| builtin_id == 1 || (cmd->redir->input_file == -1
				&& cmd->redir->output_file == -1))
			return (execute_builtin(minishell, cmd->args, status, builtin_id));
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
			status = update_exit_status(status, minishell);
			return (status);
		}
	}
}
