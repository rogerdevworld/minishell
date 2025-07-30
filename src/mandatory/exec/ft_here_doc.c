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
#include "../../../include/minishell.h"

/**
 * Expands wildcard patterns in heredoc limiters.
 * If the limiter contains wildcards and expands to multiple files,
	it's considered an ambiguous redirect error.
 * Otherwise, it updates the limiter with the expanded result.
 * Returns 0 on success, or -1 on error (e.g.,
	ambiguous redirect or allocation failure).
 */
static int	expand_heredoc_limiter(t_redir *redir)
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
	temp_args = malloc(sizeof(char *) * 2);
	if (!temp_args)
	{
		free(pattern_for_msg);
		return (-1);
	}
	temp_args[0] = ft_strdup(redir->limiter[0]);
	temp_args[1] = NULL;
	ft_wildcards(&temp_args);
	count = 0;
	while (temp_args && temp_args[count])
		count++;
	if (count > 1)
	{
		msg("ambiguous redirect", pattern_for_msg);
		free(pattern_for_msg);
		free_split(temp_args);
		return (-1);
	}
	free_split(redir->limiter);
	redir->limiter = temp_args;
	free(pattern_for_msg);
	return (0);
}

/**
 * Extracts a variable name from a string,
	handling both braced and unbraced formats.
 * Increments the pointer 'p' past the variable name.
 * Returns a newly allocated string containing the variable name.
 */
static char	*get_var_name(const char **p)
{
	const char	*start = *p;
	int			len;

	len = 0;
	if (**p == '{')
	{
		(*p)++;
		start = *p;
		while (**p && **p != '}')
		{
			(*p)++;
			len++;
		}
		if (**p == '}')
			(*p)++;
	}
	else
	{
		while (**p && (isalnum((unsigned char)**p) || **p == '_'))
		{
			(*p)++;
			len++;
		}
	}
	return (strndup(start, len));
}

/**
 * Expands environment variables and the exit status ($?) within 
	a line read from a heredoc.
 * It iterates through the line,
	identifying '$' followed by variable names or '?'.
 * Returns a newly allocated string with all variables expanded.
 */
char	*expand_line_heredoc(const char *line, int status)
{
	char		*result;
	size_t		res_len;
	char		*status_str;
	const char	*p = line;
	size_t		vlen;
	char		*var;
	char		*val;

	result = malloc(1);
	res_len = 0;
	result[0] = '\0';
	while (*p)
	{
		if (*p == '$')
		{
			p++;
			if (*p == '?')
			{
				status_str = ft_itoa(status);
				if (status_str)
				{
					vlen = strlen(status_str);
					result = realloc(result, res_len + vlen + 1);
					ft_memcpy(result + res_len, status_str, vlen);
					res_len += vlen;
					result[res_len] = '\0';
					free(status_str);
				}
				p++;
				continue ;
			}
			if (!*p || (!isalpha((unsigned char)*p) && *p != '_' && *p != '{'))
			{
				result = realloc(result, res_len + 2);
				result[res_len++] = '$';
				result[res_len] = '\0';
				continue ;
			}
			var = get_var_name(&p);
			if (var)
			{
				val = getenv(var);
				free(var);
				if (val)
				{
					vlen = strlen(val);
					result = realloc(result, res_len + vlen + 1);
					memcpy(result + res_len, val, vlen);
					res_len += vlen;
					result[res_len] = '\0';
				}
			}
		}
		else
		{
			result = realloc(result, res_len + 2);
			result[res_len++] = *p;
			result[res_len] = '\0';
			p++;
		}
	}
	return (result);
}

/**
 * Handles errors occurring during a fork operation.
 * Prints an error message to stderr and closes the 
 	provided pipe file descriptors.
 * Returns -1 to indicate an error.
 */
int	handle_fork_error(int *pipefd)
{
	perror("fork");
	close(pipefd[1]);
	close(pipefd[0]);
	return (-1);
}
/**
 * Processes the exit status of a child process.
 * Determines if the child exited normally or was terminated by a signal.
 * Specifically handles SIGINT (Ctrl+C) during heredoc input,
	returning a specific exit code.
 * Returns the child's exit code or a signal-based exit code.
 */
int	handle_child_exit(int status, int *pipefd)
{
	int	code;

	code = 0;
	if (WIFEXITED(status))
	{
		code = WEXITSTATUS(status);
	}
	else if (WIFSIGNALED(status))
	{
		code = 128 + WTERMSIG(status);
	}
	if (code == 130)
	{
		close(pipefd[0]);
		g_signal = S_HEREDOC;
		return (EXIT_SIGINT);
	}
	return (code);
}

/**
 * Child process logic for heredoc.
 * Reads lines from standard input until the limiter is encountered.
 * Expands variables in lines unless the limiter contained quotes.
 * Writes the processed lines to the pipe.
 * Exits with status 0 on success, or 1 on allocation failure.
 */
int	child_proces_heredoc(char *limiter, int *pipefd, int status)
{
	char	*line;
	char	*expanded;
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
		if (strchr(limiter, '\'') != NULL || strchr(limiter, '\"') != NULL)
		{
			write(pipefd[1], line, ft_strlen(line));
			write(pipefd[1], "\n", 1);
		}
		else
		{
			expanded = expand_line_heredoc(line, status);
			write(pipefd[1], expanded, ft_strlen(expanded));
			write(pipefd[1], "\n", 1);
			free(expanded);
		}
		free(line);
	}
	free(clean_limiter);
	close(pipefd[1]);
	exit(0);
}

/**
 * Processes all heredocs associated with a redirection structure.
 * It iterates through each heredoc limiter, forks a child process for each,
 * and sets up a pipe for the child to write to.
 * The parent process waits for the child and stores the read-end file descriptor.
 * Handles signals during heredoc input and sets the final input file for the command.
 * Returns 0 on success, or EXIT_SIGINT if a SIGINT occurs during heredoc input.
 */
int	process_all_heredocs(t_redir *redir, int s)
{
	int		i;
	int		pipefd[2];
	pid_t	pid;

	if (!redir || redir->heredoc_count <= 0 || !redir->limiter)
		return (0);
	if (expand_heredoc_limiter(redir) != 0)
		return (-1);
	if (redir->heredoc_fds)
		free(redir->heredoc_fds);
	redir->heredoc_fds = malloc(sizeof(int) * redir->heredoc_count);
	if (!redir->heredoc_fds)
		return (-1);
	for (i = 0; i < redir->heredoc_count; i++)
	{
		if (pipe(pipefd) == -1)
		{
			perror("pipe");
			return (-1);
		}
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
		close(pipefd[1]);
		g_signal = S_BASE;
		if (handle_child_exit(s, pipefd) == EXIT_SIGINT)
			return (EXIT_SIGINT);
		redir->heredoc_fds[i] = pipefd[0];
	}
	g_signal = S_BASE;
	redir->input_file = redir->heredoc_fds[redir->heredoc_count - 1];
	return (0);
}

/**
 * Recursively preprocesses heredoc redirections within the 
	Abstract Syntax Tree (AST).
 * It traverses the AST, finds all command nodes with heredocs,
	and initiates their processing.
 * Returns 0 on success, or
	-1 if a SIGINT occurs during heredoc processing in any child.
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
			{
				return (-1);
			}
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
