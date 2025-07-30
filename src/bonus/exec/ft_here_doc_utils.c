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

/**
 * Prepares a temporary array of strings for wildcard expansion,
	containing only the given limiter.
 * This is used as an input format for a potential `ft_wildcards` function.
 * @param limiter The string that will be placed into the temporary array.
 * @return A newly allocated `char**` array containing a duplicate 
	of the limiter at index 0 and NULL at index 1.
 * Returns NULL if memory allocation fails.
 */
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

/**
 * Expands wildcards in a heredoc limiter, if any exist.
 * If the limiter contains a '*', it attempts to perform wildcard expansion.
 * If the expansion results in more than one file (ambiguous redirect),
	it prints an error message.
 * If successful,
	the original limiter array is freed and replaced with the expanded result.
 * @param redir A pointer to the `t_redir` structure containing the 
	`limiter` to be expanded.
 * @return 0 on success (no expansion needed,
	or successful expansion with single result),
 * -1 on failure (memory allocation error, or ambiguous redirect).
 */
int	expand_heredoc_limiter(t_redir *redir)
{
	char	**temp_args;
	int		count;
	char	*pattern_for_msg;

	if (!redir->limiter || !redir->limiter[0] || !ft_strchr(redir->limiter[0],
			'*'))
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

/**
 * Handles an error that occurs during a fork system call.
 * It prints a perror message, closes the provided pipe file descriptors,
	and returns an error code.
 * @param pipefd An array of two integers representing the 
	read and write ends of a pipe.
 * @return -1 to indicate an error.
 */
int	handle_fork_error(int *pipefd)
{
	perror("fork");
	close(pipefd[1]);
	close(pipefd[0]);
	return (-1);
}

/**
 * Handles the exit status of a child process,
	particularly for heredoc operations.
 * It translates the raw `status` from `waitpid` into a more meaningful 
 	exit code.
 * Special handling is included for SIGINT (Ctrl+C, exit code 130),
	setting a global signal flag.
 *
 * @param status The raw status returned by `waitpid`.
 * @param pipefd An array of two integers representing the pipe file
	descriptors (used for closing on SIGINT).
 * @return The exit code of the child process,
	or a special code like `EXIT_SIGINT` if terminated by SIGINT.
 */
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
