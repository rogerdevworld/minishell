#include "../../include/minishell.h"

int	execute_command(t_command *cmd, char **envp, t_myenv *myenv,
		t_minishell *minishell, int status)
{
	int		builtin_id;
	pid_t	pid;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (1);
	builtin_id = get_builtin_cmd(cmd->args[0]);
	if (builtin_id != -1)
	{
		// Ejecutar builtin directamente sin fork ni execve
		minishell->executor->builtin_id = builtin_id;
		return (execute_builtin(minishell, cmd->args, envp, myenv, status));
	}
	// Si no es builtin, ejecutar externamente con fork + execve
	pid = fork();
	if (pid == 0)
	{
		if (cmd->input_file != -1)
			dup2(cmd->input_file, STDIN_FILENO);
		if (cmd->output_file != -1)
			dup2(cmd->output_file, STDOUT_FILENO);
		resolve_command_path(cmd, envp);
		execve(cmd->path, cmd->args, envp);
		// perror("execve failed");
		exit(1);
	}
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}

int	execute_pipe(t_ast *node, char **envp, t_myenv *myenv,
		t_minishell *minishell)
{
	int	fds[2];
	int	status;

	pid_t pid1, pid2;
	if (pipe(fds) == -1)
		return (1);
	pid1 = fork();
	if (pid1 == 0)
	{
		close(fds[0]);
		dup2(fds[1], STDOUT_FILENO);
		exit(execute_ast(node->left, envp, myenv, minishell, status));
	}
	pid2 = fork();
	if (pid2 == 0)
	{
		close(fds[1]);
		dup2(fds[0], STDIN_FILENO);
		exit(execute_ast(node->right, envp, myenv, minishell, status));
	}
	close(fds[0]);
	close(fds[1]);
	waitpid(pid1, &status, 0);
	waitpid(pid2, &status, 0);
	return (WEXITSTATUS(status));
}

int	execute_and(t_ast *node, char **envp, t_myenv *myenv,
		t_minishell *minishell)
{
	int	status;

	status = execute_ast(node->left, envp, myenv, minishell, status);
	if (status == 0)
		status = execute_ast(node->right, envp, myenv, minishell, status);
	return (status);
}

int	execute_or(t_ast *node, char **envp, t_myenv *myenv, t_minishell *minishell)
{
	int	status;

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

	pid = fork();
	if (pid == 0)
		exit(execute_ast(node->left, envp, myenv, minishell, status));
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}

int	execute_ast(t_ast *node, char **envp, t_myenv *myenv,
		t_minishell *minishell, int status)
{
	if (!node)
		return (1);
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
