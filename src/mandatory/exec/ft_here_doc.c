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

static int	handle_single_heredoc(t_redir *redir, int i, int s)
{
	int		pipefd[2];
	pid_t	pid;

	if (pipe(pipefd) == -1)
		return (perror("pipe"), -1);
	g_signal = S_HEREDOC;
	signal(SIGINT, ft_sigint);
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
