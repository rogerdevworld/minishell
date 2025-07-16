#include "../../include/minishell.h"

int execute_command(t_command *cmd, char **envp, t_myenv *myenv,
					t_minishell *minishell, int status)
{
	int builtin_id;
	pid_t pid;
	char **clean_args;

	if (!cmd->redir->limiter)
		ft_wildcards(&(cmd->args));
	if (!cmd || !cmd->args || !cmd->args[0])
		return (1);
	builtin_id = get_builtin_cmd(cmd->args[0]);
	if (builtin_id != -1)
	{
		minishell->executor->builtin_id = builtin_id;
		return (execute_builtin(minishell, cmd->args, myenv, status));
	}
	g_signal = S_CMD;
	pid = fork();
	if (pid == 0)
	{
		set_defaul_signals();

		if (cmd->redir)
		{
			if (cmd->redir->limiter)
			{
				if (process_heredoc(cmd->redir) == -1)
					exit(1);
			}
			if (cmd->redir->input_file == -1 || cmd->redir->output_file == -1)
			{
				ft_printf(" No such file or directory\n");
				exit(1);
			}
			if (cmd->redir->input_file != -1)
				dup2(cmd->redir->input_file, STDIN_FILENO);
			if (cmd->redir->output_file != -1)
				dup2(cmd->redir->output_file, STDOUT_FILENO);
		}
		resolve_command_path(cmd, envp);
		clean_args = remove_quotes_from_args(cmd->args);
		free_split(cmd->args);
		cmd->args = clean_args;
		execve(cmd->path, cmd->args, envp);
		exit(1);
	}
	waitpid(pid, &status, 0);
	status = update_exit_status(status, minishell);
	// return (WEXITSTATUS(status));
	return (status);
}

int execute_pipe(t_ast *node, char **envp, t_myenv *myenv,
				 t_minishell *minishell)
{
	int fds[2];
	int status;

	pid_t pid1, pid2;
	if (pipe(fds) == -1)
		return (1);
	g_signal = S_CMD;
	pid1 = fork();
	status = 0;
	if (pid1 == 0)
	{
		set_defaul_signals();
		close(fds[0]);
		dup2(fds[1], STDOUT_FILENO);
		exit(execute_ast(node->left, envp, myenv, minishell, status));
	}
	pid2 = fork();
	if (pid2 == 0)
	{
		set_defaul_signals();
		close(fds[1]);
		dup2(fds[0], STDIN_FILENO);
		exit(execute_ast(node->right, envp, myenv, minishell, status));
	}
	close(fds[0]);
	close(fds[1]);
	waitpid(pid1, &status, 0);
	update_exit_status(status, minishell);
	waitpid(pid2, &status, 0);
	status = update_exit_status(status, minishell);
	// return (WEXITSTATUS(status));
	return (status);
}

int execute_and(t_ast *node, char **envp, t_myenv *myenv,
				t_minishell *minishell)
{
	int status;

	status = 0;
	status = execute_ast(node->left, envp, myenv, minishell, status);
	if (status == 0)
		status = execute_ast(node->right, envp, myenv, minishell, status);
	return (status);
}

int execute_or(t_ast *node, char **envp, t_myenv *myenv, t_minishell *minishell)
{
	int status;

	status = 0;
	status = execute_ast(node->left, envp, myenv, minishell, status);
	if (status != 0)
		status = execute_ast(node->right, envp, myenv, minishell, status);
	return (status);
}

int execute_subshell(t_ast *node, char **envp, t_myenv *myenv,
					 t_minishell *minishell)
{
	pid_t pid;
	int status;

	g_signal = S_CMD;
	pid = fork();
	status = 0;
	if (pid == 0)
	{
		set_defaul_signals();
		exit(execute_ast(node->left, envp, myenv, minishell, status));
	}
	waitpid(pid, &status, 0);
	status = update_exit_status(status, minishell);
	// return (WEXITSTATUS(status));
	return (status);
}


int	process_heredoc(t_redir *redir)
{
	int		pipe_fds[2];
	char	*line;

	if (!redir || !redir->limiter)
		return (0);
	if (pipe(pipe_fds) == -1)
	{
		perror("pipe");
		return (-1);
	}
	while (1)
	{
		line = readline("> ");
		if (!line)
			break;
		if (ft_strncmp(line, redir->limiter, ft_strlen(redir->limiter) + 1) == 0)
		{
			free(line);
			break;
		}
		write(pipe_fds[1], line, ft_strlen(line));
		write(pipe_fds[1], "\n", 1);
		free(line);
	}
	close(pipe_fds[1]);
	redir->input_file = pipe_fds[0];
	return (0);
}



int execute_ast(t_ast *node, char **envp, t_myenv *myenv,
				t_minishell *minishell, int status)
{
	if (!node)
		return (0);
	if (node->type == NODE_COMMAND)
		return (execute_command(node->cmd, envp, myenv, minishell, status));
	else if (node->type == NODE_PIPE)
		return (execute_pipe(node, envp, myenv, minishell));
	else if (node->type == NODE_AND)
		return (execute_and(node, envp, myenv, minishell));
	else if (node->type == NODE_OR)
		return (execute_or(node, envp, myenv, minishell));
	else if (node->type == NODE_SUBSHELL)
		return (execute_subshell(node, envp, myenv, minishell));
	return (1);
}

void print_args(char **args)
{
	int i = 0;
	while (args[i] != NULL)
	{
		printf("Argument %d: %s\n", i, args[i]);
		i++;
	}
}