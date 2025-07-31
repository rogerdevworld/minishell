/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_here_doc_utils.c                                 :+:      :+:    :+:  */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:56:13 by rmarrero          #+#    #+#             */
/*   Updated: 2025/03/27 11:57:56 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static char	**prepare_wildcard_expansion(char *limiter)
{
	char	**temp_args;

	temp_args = malloc(sizeof(char *) * 2);
	if (!temp_args)
		return (NULL);
	temp_args[0] = ft_strdup(limiter);
	temp_args[1] = NULL;
	return (temp_args);
}

int	expand_heredoc_limiter(t_redir *redir)
{
	char	**temp_args;
	int		count;
	char	*pattern_for_msg;

	if (!redir->limiter || !redir->limiter[0]
		|| !ft_strchr(redir->limiter[0], '*'))
		return (0);
	pattern_for_msg = ft_strdup(redir->limiter[0]);
	if (!pattern_for_msg)
		return (-1);
	temp_args = prepare_wildcard_expansion(redir->limiter[0]);
	if (!temp_args)
		return (free(pattern_for_msg), -1);
	ft_wildcards(&temp_args);
	count = 0;
	while (temp_args && temp_args[count])
		count++;
	if (count > 1)
		return (msg("ambiguous redirect", pattern_for_msg),
			free(pattern_for_msg), free_split(temp_args), -1);
	free_split(redir->limiter);
	redir->limiter = temp_args;
	free(pattern_for_msg);
	return (0);
}

int	handle_fork_error(int *pipefd)
{
	perror("fork");
	close(pipefd[1]);
	close(pipefd[0]);
	return (-1);
}

int	handle_child_exit(int status, int *pipefd)
{
	int	code;

	code = 0;
	if (WIFEXITED(status))
		code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		code = 128 + WTERMSIG(status);
	if (code == 130)
	{
		close(pipefd[0]);
		g_signal = S_HEREDOC;
		return (EXIT_SIGINT);
	}
	return (code);
}
