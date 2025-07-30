/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_here_doc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:56:13 by rmarrero          #+#    #+#             */
/*   Updated: 2025/03/27 11:57:56 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../include/minishell.h"

/**
 * @brief Writes a line to the heredoc pipe, handling expansion if necessary.
 *
 * This function determines whether the line read from the user should be
 * expanded for environment variables before being written to the pipe.
 * Expansion is skipped if the limiter contained quotes.
 *
 * @param fd The file descriptor of the pipe to write to.
 * @param line The line read from the user.
 * @param limiter The heredoc delimiter.
 * @param status The exit status of the previous command, used for expansion.
 */
static void	write_heredoc_line(int fd, char *line, char *limiter, int status)
{
	char	*expanded;

	if (strchr(limiter, '\'') != NULL || strchr(limiter, '\"') != NULL)
	{
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
	}
	else
	{
		expanded = expand_line_heredoc(line, status);
		write(fd, expanded, ft_strlen(expanded));
		write(fd, "\n", 1);
		free(expanded);
	}
	free(line);
}

/**
 * @brief Handles the child process for a single heredoc.
 *
 * This function sets up signal handlers, reads lines from standard input
 * until the limiter is encountered, and writes them to the pipe.
 * Environment variable expansion is performed based on the limiter's quotes.
 *
 * @param limiter The heredoc delimiter.
 * @param pipefd An array of two integers representing the pipe file descriptors.
 * @param status The exit status of the previous command.
 * @return Returns 0 on success, or exits with an error code on failure.
 */
static int	child_proces_heredoc(char *limiter, int *pipefd, int status)
{
	char	*line;
	char	*clean_limiter;

	close(pipefd[0]);
	signals_heredoc();
	line = NULL;
	clean_limiter = remove_all_quotes(limiter);
	if (!clean_limiter)
		exit(1);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, clean_limiter) == 0)
		{
			free(line);
			break ;
		}
		write_heredoc_line(pipefd[1], line, limiter, status);
	}
	free(clean_limiter);
	close(pipefd[1]);
	exit(0);
}

/**
 * @brief Handles the creation and processing of a single heredoc.
 *
 * This function creates a pipe, forks a child process to handle the heredoc
 * input, and waits for the child process to complete. It also manages
 * signal handling during the heredoc input phase.
 *
 * @param redir A pointer to the redirection structure.
 * @param i The index of the current heredoc in the redir->limiter array.
 * @param s The exit status of the previous command.
 * @return Returns 0 on success, -1 on pipe or fork error, or EXIT_SIGINT if
 * the child process was interrupted by a signal.
 */
static int	handle_single_heredoc(t_redir *redir, int i, int s)
{
	int		pipefd[2];
	pid_t	pid;

	if (pipe(pipefd) == -1)
		return (perror("pipe"), -1);
	g_signal = S_HEREDOC;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	pid = fork();
	if (pid == -1)
		return (handle_fork_error(pipefd));
	if (pid == 0)
		child_proces_heredoc(redir->limiter[i], pipefd, s);
	close(pipefd[1]);
	waitpid(pid, &s, 0);
	g_signal = S_BASE;
	if (handle_child_exit(s, pipefd) == EXIT_SIGINT)
		return (EXIT_SIGINT);
	redir->heredoc_fds[i] = pipefd[0];
	return (0);
}

/**
 * @brief Processes all heredocs for a given redirection structure.
 *
 * This function iterates through all heredoc delimiters, creating a pipe
 * and a child process for each to handle the heredoc input. The file
 * descriptors for the heredocs are stored in `redir->heredoc_fds`.
 * @param redir A pointer to the redirection structure containing
  	heredoc information.
 * @param s The exit status of the previous command.
 * @return Returns 0 on success, -1 on memory allocation or expansion error,
 * or a non-zero value if any `handle_single_heredoc` call fails.
 */
int	process_all_heredocs(t_redir *redir, int s)
{
	int	i;
	int	result;

	if (expand_heredoc_limiter(redir) != 0)
		return (-1);
	if (redir->heredoc_fds)
		free(redir->heredoc_fds);
	redir->heredoc_fds = malloc(sizeof(int) * redir->heredoc_count);
	if (!redir->heredoc_fds)
		return (-1);
	i = 0;
	while (i < redir->heredoc_count)
	{
		result = handle_single_heredoc(redir, i, s);
		if (result != 0)
			return (result);
		i++;
	}
	g_signal = S_BASE;
	redir->input_file = redir->heredoc_fds[redir->heredoc_count - 1];
	return (0);
}

/**
 * @brief Recursively preprocesses heredocs within an AST.
 *
 * This function traverses the Abstract Syntax Tree (AST) and identifies
 * command nodes that contain heredoc redirections. For each such node, it calls
 * `process_all_heredocs` to handle the heredoc input.
 *
 * @param node A pointer to the current node in the AST.
 * @param status The exit status of the previous command.
 * @return Returns 0 on success, or -1 if any heredoc processing fails.
 */
int	preprocess_heredocs(t_ast *node, int status)
{
	if (!node)
		return (0);
	if (node->type == NODE_COMMAND)
	{
		if (node->cmd && node->cmd->redir
			&& node->cmd->redir->heredoc_count > 0)
		{
			if (process_all_heredocs(node->cmd->redir, status) == 130)
				return (-1);
		}
	}
	else if (node->type == NODE_PIPE || node->type == NODE_AND
		|| node->type == NODE_OR)
	{
		if (preprocess_heredocs(node->left, status) == -1)
			return (-1);
		if (preprocess_heredocs(node->right, status) == -1)
			return (-1);
	}
	else if (node->type == NODE_SUBSHELL)
	{
		if (preprocess_heredocs(node->left, status) == -1)
			return (-1);
	}
	return (0);
}
