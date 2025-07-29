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
 * creo que esto no lo estoy usando para heredocs
 *
 */
// int	process_single_heredoc(char *limiter, int *fd_out)
// {
// 	int		pipe_fds[2];
// 	char	*line;

// 	if (pipe(pipe_fds) == -1)
// 	{
// 		perror("pipe");
// 		return (-1);
// 	}
// 	/**aqui funciona pero no termina de salir del heredoc... */
// 	while (1)
// 	{
// 		line = readline("> ");
// 		if (g_signal == S_SIGINT_CMD)
// 		{
// 			free(line);
// 			close(pipe_fds[1]);
// 			close(pipe_fds[0]);
// 			return (-1);              // error por interrupción
// 		}
// 		if (!line || ft_strncmp(line, limiter, ft_strlen(limiter) + 1) == 0)
// 		{
// 			free(line);
// 			break ;
// 		}
// 		write(pipe_fds[1], line, ft_strlen(line));
// 		write(pipe_fds[1], "\n", 1);
// 		free(line);
// 	}
// 	close(pipe_fds[1]);
// 	*fd_out = pipe_fds[0];
// 	//*fd_out = 0;
// 	g_signal = S_BASE;
// 	return (0);
// }

/**
 * tratando de hacer las expansiones, funcionan pero cuando estan solas, no con
 * mas palabras
 * ej : >$PWD -> funciona
 * ej : >hola $PWD -> no funciona
 */

// src/exec/ft_here_doc.c

static int	expand_heredoc_limiter(t_redir *redir)
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

static char	*get_var_name(const char **p)
{
	const char	*start = *p;
	int			len;

	len = 0;
	if (**p == '{')
	{
		(*p)++; // saltar '{'
		start = *p;
		while (**p && **p != '}')
		{
			(*p)++;
			len++;
		}
		// si cierra '}'
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
 * esta funciona para expandir variables con texto
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

	// size_t len = strlen(line);
	result = malloc(1);
	res_len = 0;
	result[0] = '\0';
	while (*p)
	{
		if (*p == '$')
		{
			p++; // avanzar después de $
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
				p++; // saltar '?'
				continue ;
			}
			if (!*p || (!isalpha((unsigned char)*p) && *p != '_' && *p != '{'))
			{
				// no es una variable válida, copiar literal '$'
				result = realloc(result, res_len + 2);
				result[res_len++] = '$';
				result[res_len] = '\0';
				continue ;
			}
			// obtener nombre de variable
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
			// copiar caracter normal
			result = realloc(result, res_len + 2);
			result[res_len++] = *p;
			result[res_len] = '\0';
			p++;
		}
	}
	return (result);
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

// src/exec/ft_here_doc.c

int	child_proces_heredoc(char *limiter, int *pipefd, int status)
{
	char	*line;
	char	*expanded;

	char *clean_limiter; // Variable para el limiter sin comillas
	close(pipefd[0]);
	signals_heredoc();
	line = NULL;
	// --- INICIO DE LA CORRECCIÓN ---
	clean_limiter = remove_all_quotes(limiter); // Creamos la copia sin comillas
	if (!clean_limiter)
		exit(1); // Salir si falla el malloc
	// --- FIN DE LA CORRECCIÓN ---
	while (1)
	{
		line = readline("> ");
		// Usamos la copia sin comillas para la comparación
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
	// --- INICIO DE LA CORRECCIÓN ---
	free(clean_limiter); // Liberamos la copia
	// --- FIN DE LA CORRECCIÓN ---
	close(pipefd[1]);
	exit(0);
}

int	process_all_heredocs(t_redir *redir, int s)
{
	int		i;
	int		pipefd[2];
	pid_t	pid;

	if (!redir || redir->heredoc_count <= 0 || !redir->limiter)
		return (0);
	if (expand_heredoc_limiter(redir) != 0)
		return (-1); // Si la expansión falla, detenemos la ejecución.
	// --- INICIO DE LA CORRECCIÓN ---
	// Liberamos el array pre-asignado para evitar una fuga de memoria.
	if (redir->heredoc_fds)
		free(redir->heredoc_fds);
	// --- FIN DE LA CORRECCIÓN ---
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

// Procesa recursivamente todos los heredocs del AST antes de ejecutar
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

