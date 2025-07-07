/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:05:47 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/03 14:43:09 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

/**
 * nueva con solo dos parametros
 */
/* static void	execute_builtin_no_pipe(t_minishell *minishell, t_command *cmd)
{
	int			saved_stdin;
	int			saved_stdout;
	int			builtin_result;
	t_executor	*exec;

	exec = minishell->executor;
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
	builtin_result = execute_builtin(exec->builtin_id, cmd->args, exec->envp,
			exec->myenv);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	minishell->exit = builtin_result;
} */

/**
 * version vieja con 5 parametros
 */

static void	execute_builtin_no_pipe(t_minishell *minishell, t_executor *exec,
		t_command *cmd, char **envp, t_myenv *myenv)
{
	int	saved_stdin;
	int	saved_stdout;
	int	builtin_result;

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
	builtin_result = execute_builtin(exec->builtin_id, cmd->args, envp, myenv);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	minishell->exit = builtin_result;
}


/**
 * version nueva, solo 3 parametros
 */
/* static void	child_process(t_minishell *minishell, t_command *cmd, int pipefd[2])
{
	char		*path;
	t_executor	*exec;

	exec = minishell->executor;
	path = NULL;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (cmd->input_file != -1)
	{
		dup2(cmd->input_file, STDIN_FILENO);
		close(cmd->input_file);
	}
	else if (exec->prev_fd != -1)
	{
		dup2(exec->prev_fd, STDIN_FILENO);
		close(exec->prev_fd);
	}
	if (cmd->output_file != -1)
	{
		dup2(cmd->output_file, STDOUT_FILENO);
		close(cmd->output_file);
	}
	else if (cmd->operator== PIPE)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
	}
	if (exec->builtin_id != -1)
		exit(execute_builtin(exec->builtin_id, cmd->args, exec->envp,
				exec->myenv));
	if (cmd->args[0][0] == '/' || cmd->args[0][0] == '.')
		path = ft_strdup(cmd->args[0]);
	else
		path = get_path(cmd->args[0], exec->envp);
	if (!path || access(path, X_OK) != 0)
	{
		ft_putstr_fd(cmd->args[0], 2);
		write(2, ": command not found\n", 21);
		free(path);
		exit(EXIT_CMD_NOT_FOUND);
	}
	execve(path, cmd->args, exec->myenv->env);
	exit(EXIT_CMD_NOT_FOUND);
} */

/**
 * version antigua, muchos parametros
 */

static void	child_process(t_executor *exec, t_command *cmd, char **envp,
		t_myenv *myenv, int pipefd[2])
{
	char	*path;

	path = NULL;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (cmd->input_file != -1)
	{
		dup2(cmd->input_file, STDIN_FILENO);
		close(cmd->input_file);
	}
	else if (exec->prev_fd != -1)
	{
		dup2(exec->prev_fd, STDIN_FILENO);
		close(exec->prev_fd);
	}
	if (cmd->output_file != -1)
	{
		dup2(cmd->output_file, STDOUT_FILENO);
		close(cmd->output_file);
	}
	else if (cmd->operator== PIPE)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
	}
	if (exec->builtin_id != -1)
		exit(execute_builtin(exec->builtin_id, cmd->args, envp, myenv));
	if (cmd->args[0][0] == '/' || cmd->args[0][0] == '.')
		path = ft_strdup(cmd->args[0]);
	else
		path = get_path(cmd->args[0], envp);
	if (!path || access(path, X_OK) != 0)
	{
		ft_putstr_fd(cmd->args[0], 2);
		write(2, ": command not found\n", 21);
		free(path);
		exit(EXIT_CMD_NOT_FOUND);
	}
	execve(path, cmd->args, myenv->env);
	exit(EXIT_CMD_NOT_FOUND);
}


/**
 * version nueva menos parametros
 */
/* static void	parent_process(t_minishell *minishell, t_command *cmd,
		int pipefd[2], pid_t pid)
{
	int			status;
	int			signo;
	t_executor	*exec;

	exec = minishell->executor;
	if (exec->prev_fd != -1)
		close(exec->prev_fd);
	if (cmd->operator== PIPE)
	{
		close(pipefd[1]);
		exec->prev_fd = pipefd[0];
	}
	else
		exec->prev_fd = -1;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		minishell->exit = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		signo = WTERMSIG(status);
		if (signo == SIGPIPE)
			minishell->exit = 0;
		else if (signo == SIGINT)
		{
			minishell->exit = 130;
			write(1, "\n", 1);
		}
		else if (signo == SIGQUIT)
		{
			minishell->exit = 131;
			write(1, "Quit (core dumped)\n", 19);
		}
		else
			minishell->exit = 128 + signo;
	}
	g_signal = S_BASE;
} */

/**
 * version vieja demasiados parametros
 */

static void	parent_process(t_minishell *minishell, t_executor *exec,
		t_command *cmd, int pipefd[2], pid_t pid)
{
	int	status;
	int	signo;

	if (exec->prev_fd != -1)
		close(exec->prev_fd);
	if (cmd->operator== PIPE)
	{
		close(pipefd[1]);
		exec->prev_fd = pipefd[0];
	}
	else
		exec->prev_fd = -1;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		minishell->exit = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		signo = WTERMSIG(status);
		if (signo == SIGPIPE)
			minishell->exit = 0;
		else if (signo == SIGINT)
		{
			minishell->exit = 130;
			write(1, "\n", 1);
		}
		else if (signo == SIGQUIT)
		{
			minishell->exit = 131;
			write(1, "Quit (core dumped)\n", 19);
		}
		else
			minishell->exit = 128 + signo;
	}
	g_signal = S_BASE;
}

/**
 * nueva funcion recibiendo solo 2 parametros
 */
/* void	ft_check_executor_single(t_minishell *minishell, t_command *cmd)
{
	int			pipefd[2];
	pid_t		pid;
	t_executor	*exec;

	exec = minishell->executor;
	if (cmd->limiter)
		ft_here_doc(cmd->limiter);
	if (!cmd->args || !cmd->args[0])
		return ;
	exec->builtin_id = get_builtin_cmd(cmd->args[0]);
	if (exec->builtin_id != -1 && cmd->operator!= PIPE && exec->prev_fd == - 1)
	{
		execute_builtin_no_pipe(minishell, cmd);
		return ;
	}
	if (cmd->operator== PIPE)
	{
		if (pipe(pipefd) == -1)
			ft_exit("pipe failed");
	}
	g_signal = S_CMD;
	pid = fork();
	if (pid == -1)
		ft_exit("fork failed");
	if (pid == 0)
		child_process(minishell, cmd, pipefd);
	else
		parent_process(minishell, cmd, pipefd, pid);
} */
/**
 * vieja funcion recibiendo 5 parametros
 */

void	ft_check_executor_single(t_minishell *minishell, t_executor *exec,
		t_command *cmd, char **envp, t_myenv *myenv)
{
	int		pipefd[2];
	pid_t	pid;

	if (cmd->limiter)
		ft_here_doc(cmd->limiter);
	if (!cmd->args || !cmd->args[0])
		return ;
	exec->builtin_id = get_builtin_cmd(cmd->args[0]);
	if (exec->builtin_id != -1 && cmd->operator!= PIPE && exec->prev_fd == - 1)
	{
		execute_builtin_no_pipe(minishell, exec, cmd, envp, myenv);
		return ;
	}
	if (cmd->operator == PIPE)
	{
		if (pipe(pipefd) == -1)
			ft_exit("pipe failed");
	}
	g_signal = S_CMD;
	pid = fork();
	if (pid == -1)
		ft_exit("fork failed");
	if (pid == 0)
		child_process(exec, cmd, envp, myenv, pipefd);
	else
		parent_process(minishell, exec, cmd, pipefd, pid);
}

/**
 * nueva funcion de executer recibiendo solo un parametro
 */
/* int	execute_command_list(t_minishell *minishell)
{
	t_command	*current;

	current = minishell->cmd;
	while (current)
	{
		if (ft_strcmp(current->args[0], "&&") == 0)
		{
			if (minishell->exit != 0)
				current = current->next;
			current = current->next;
			continue ;
		}
		else if (ft_strcmp(current->args[0], "||") == 0)
		{
			if (minishell->exit == 0)
				current = current->next;
			current = current->next;
			continue ;
		}
		ft_check_executor_single(minishell, current);
		current = current->next;
	}
	if (minishell)
		return (minishell->exit);
	return (0);
} */
/**
 * funcion antigua con 5 parametros de entrada, se cambia para
 * que solo reciba la estructura t_minishell
 */

int	execute_command_list(t_minishell *minishell, t_executor *exec,
		t_command *cmd, char **envp, t_myenv *myenv)
{
	t_command	*current;

	current = cmd;
	while (current)
	{
		if (ft_strcmp(current->args[0], "&&") == 0)
		{
			if (minishell->exit != 0)
				current = current->next;
			current = current->next;
			continue ;
		}
		else if (ft_strcmp(current->args[0], "||") == 0)
		{
			if (minishell->exit == 0)
				current = current->next;
			current = current->next;
			continue ;
		}
		ft_check_executor_single(minishell, exec, current, envp, myenv);
		current = current->next;
	}
	if (minishell)
		return (minishell->exit);
	return (0);
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
