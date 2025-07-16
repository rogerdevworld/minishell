#include "../../include/minishell.h"

int	execute_command(t_command *cmd, char **envp, t_myenv *myenv,
		t_minishell *minishell, int status)
{
	int		builtin_id;
	pid_t	pid;
	char	**clean_args;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (1);
	builtin_id = get_builtin_cmd(cmd->args[0]);
	if (builtin_id == -1)
	{
		g_signal = S_CMD;
		pid = fork();
		if (pid == 0)
		{
			set_defaul_signals();
			if (cmd->redir)
			{
				if (process_all_heredocs(cmd->redir) == -1)
					exit(1);
				if (handle_output_redirections(cmd->redir) == -1)
					exit(1);
				if (handle_input_redirection(cmd->redir) == -1)
					exit(1);
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
			perror("execve");
			exit(1);
		}
		waitpid(pid, &status, 0);
		status = update_exit_status(status, minishell);
		return (status);
	}
	else // Es builtin
	{
		if (!cmd->redir || (cmd->redir->input_file == -1
				&& cmd->redir->output_file == -1))
		{
			minishell->executor->builtin_id = builtin_id;
			return (execute_builtin(minishell, cmd->args, myenv, status));
		}
		else
		{
			g_signal = S_CMD;
			pid = fork();
			if (pid == 0)
			{
				set_defaul_signals();
				if (process_all_heredocs(cmd->redir) == -1)
					exit(1);
				if (handle_output_redirections(cmd->redir) == -1)
					exit(1);
				if (handle_input_redirection(cmd->redir) == -1)
					exit(1);

				if (cmd->redir->input_file != -1)
					dup2(cmd->redir->input_file, STDIN_FILENO);
				if (cmd->redir->output_file != -1)
					dup2(cmd->redir->output_file, STDOUT_FILENO);
				minishell->executor->builtin_id = builtin_id;
				exit(execute_builtin(minishell, cmd->args, myenv, status));
			}
			waitpid(pid, &status, 0);
			status = update_exit_status(status, minishell);
			return (status);
		}
	}
}

int	execute_pipe(t_ast *node, char **envp, t_myenv *myenv,
		t_minishell *minishell)
{
	int	fds[2];
	int	status;

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

int	execute_and(t_ast *node, char **envp, t_myenv *myenv,
		t_minishell *minishell)
{
	int	status;

	status = 0;
	status = execute_ast(node->left, envp, myenv, minishell, status);
	if (status == 0)
		status = execute_ast(node->right, envp, myenv, minishell, status);
	return (status);
}

int	execute_or(t_ast *node, char **envp, t_myenv *myenv, t_minishell *minishell)
{
	int	status;

	status = 0;
	status = execute_ast(node->left, envp, myenv, minishell, status);
	if (status != 0)
		status = execute_ast(node->right, envp, myenv, minishell, status);
	return (status);
}

int	execute_subshell(t_ast *node, char **envp, t_myenv *myenv,
		t_minishell *minishell)
{
	pid_t	pid;
	int		status;

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

int	process_single_heredoc(char *limiter, int *fd_out)
{
	int		pipe_fds[2];
	char	*line;

	if (pipe(pipe_fds) == -1)
	{
		perror("pipe");
		return (-1);
	}
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strncmp(line, limiter, ft_strlen(limiter) + 1) == 0)
		{
			free(line);
			break ;
		}
		write(pipe_fds[1], line, ft_strlen(line));
		write(pipe_fds[1], "\n", 1);
		free(line);
	}
	close(pipe_fds[1]);
	*fd_out = pipe_fds[0];
	return (0);
}
int	process_all_heredocs(t_redir *redir)
{
	int	i;

	if (!redir || redir->heredoc_count == 0)
		return (0);
	i = 0;
	while (i < redir->heredoc_count)
	{
		if (process_single_heredoc(redir->limiter[i], &redir->heredoc_fds[i]) ==
			-1)
			return (-1);
		i++;
	}
	// Usar el último heredoc como input_file
	redir->input_file = redir->heredoc_fds[redir->heredoc_count - 1];
	return (0);
}

int	handle_output_redirections(t_redir *redir)
{
	int		i;
	int		fd;
	char	**files;

	i = 0;
	files = redir->out_file;
	while (files && files[i])
	{
		if (redir->output_file != -1)
			close(redir->output_file);
		fd = open(files[i], O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (fd == -1)
		{
			perror("open");
			return (-1);
		}
		redir->output_file = fd;
		i++;
	}
	return (0);
}

int	handle_input_redirection(t_redir *redir)
{
	int		fd;
	char	**files;
	int		i;

	if (!redir->in_file)
		return (0);
	files = redir->in_file;
	i = 0;
	while (files[i])
	{
		fd = open(files[i], O_RDONLY);
		if (fd == -1)
		{
			perror(files[i]); // Mostrar el error
			return (-1);      // Error si CUALQUIERA falla
		}
		if (redir->input_file != -1)
			close(redir->input_file);
		redir->input_file = fd;
		i++;
	}
	return (0);
}


int	execute_ast(t_ast *node, char **envp, t_myenv *myenv,
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

void	print_args(char **args)
{
	int i = 0;
	while (args[i] != NULL)
	{
		printf("Argument %d: %s\n", i, args[i]);
		i++;
	}
}