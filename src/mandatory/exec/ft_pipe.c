/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 20:16:29 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/16 20:16:34 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../include/minishell.h"

/**
 * @brief Executes the left-hand side of a pipe in a child process.
 *
 * This function is responsible for the child process that handles the
 * left command in a pipe. It redirects its standard output to the write
 * end of the pipe and then executes the command.
 * @param node A pointer to the AST node representing the pipe.
 * @param myenv A pointer to the environment variables.
 * @param minishell A pointer to the minishell structure.
 * @param fds An array containing the read and write file
	descriptors of the pipe.
 * @return This function exits with the status of the executed command.
 */
static int	child_process_left(t_ast *node, t_myenv *myenv,
		t_minishell *minishell, int *fds)
{
	set_defaul_signals();
	close(fds[0]);
	dup2(fds[1], STDOUT_FILENO);
	exit(execute_ast(node->left, myenv, minishell, 0));
}

/**
 * @brief Executes the right-hand side of a pipe in a child process.
 *
 * This function is responsible for the child process that handles the
 * right command in a pipe. It redirects its standard input to the read
 * end of the pipe and then executes the command.
 * @param node A pointer to the AST node representing the pipe.
 * @param myenv A pointer to the environment variables.
 * @param minishell A pointer to the minishell structure.
 * @param fds An array containing the read and write file descriptors
	of the pipe.
 * @return This function exits with the status of the executed command.
 */
static int	child_process_right(t_ast *node, t_myenv *myenv,
		t_minishell *minishell, int *fds)
{
	set_defaul_signals();
	close(fds[1]);
	dup2(fds[0], STDIN_FILENO);
	exit(execute_ast(node->right, myenv, minishell, 0));
}

/**
 * @brief Executes a pipe command.
 *
 * This function creates a pipe, forks two child processes (one for the left
 * command and one for the right command), and sets up the redirection
 * between them. It then waits for both child processes to complete and
 * updates the minishell's exit status.
 *
 * @param node A pointer to the AST node representing the pipe.
 * @param myenv A pointer to the environment variables.
 * @param minishell A pointer to the minishell structure.
 * @return The exit status of the last command in the pipe.
 */
int	execute_pipe(t_ast *node, t_myenv *myenv, t_minishell *minishell)
{
	int		fds[2];
	int		status;
	pid_t	pid1;
	pid_t	pid2;

	if (pipe(fds) == -1)
		return (1);
	g_signal = S_CMD;
	pid1 = fork();
	if (pid1 == 0)
		child_process_left(node, myenv, minishell, fds);
	pid2 = fork();
	if (pid2 == 0)
		child_process_right(node, myenv, minishell, fds);
	close(fds[0]);
	close(fds[1]);
	waitpid(pid1, &status, 0);
	update_exit_status(status, minishell);
	waitpid(pid2, &status, 0);
	return (update_exit_status(status, minishell));
}
