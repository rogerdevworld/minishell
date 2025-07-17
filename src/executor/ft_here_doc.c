/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:05:47 by rmarrero          #+#    #+#             */
/*   Updated: 2025/04/01 13:05:50 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

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
		if (process_single_heredoc(redir->limiter[i], \
			&redir->heredoc_fds[i]) == -1)
			return (-1);
		i++;
	}
	redir->input_file = redir->heredoc_fds[redir->heredoc_count - 1];
	return (0);
}
