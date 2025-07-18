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
static char *get_var_name(const char **p)
{
    const char *start = *p;
    int len = 0;

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
    return strndup(start, len);
}

/**
 * esta funciona para expandir variables con texto 
 */
char *expand_line_heredoc(const char *line)
{
    //size_t len = strlen(line);
    char *result = malloc(1);
    size_t res_len = 0;
    result[0] = '\0';

    const char *p = line;
    while (*p)
    {
        if (*p == '$')
        {
            p++; // avanzar después de $
            if (!*p || (!isalpha((unsigned char)*p) && *p != '_' && *p != '{'))
            {
                // no es una variable válida, copiar literal '$'
                result = realloc(result, res_len + 2);
                result[res_len++] = '$';
                result[res_len] = '\0';
                continue;
            }
            // obtener nombre de variable
            char *var = get_var_name(&p);
            if (var)
            {
                char *val = getenv(var);
                free(var);
                if (val)
                {
                    size_t vlen = strlen(val);
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
    return result;
}

int	process_all_heredocs(t_redir *redir)
{
	int i;
	int pipefd[2];
	char	*expanded;

	if (!redir || redir->heredoc_count <= 0 || !redir->limiter)
		return (0);

	//ft_printf("\nDEBUG= el limiter es: %s\n", redir->limiter[0]);
	// reserva espacio para fds
	redir->heredoc_fds = malloc(sizeof(int) * redir->heredoc_count);
	if (!redir->heredoc_fds)
		return (-1);
    //set_defaul_signals();
	for (i = 0; i < redir->heredoc_count; i++)
	{
		if (pipe(pipefd) == -1)
		{
			perror("pipe");
			return (-1);
		}
        g_signal = S_HEREDOC;
        signal(SIGINT, ft_sigint);
	    signal(SIGQUIT, SIG_IGN);
		// pide input hasta el limiter
		char *line = NULL;
		while (1)
		{
			line = readline("> ");
            if (g_signal == S_SIGINT_CMD)
            {
                free(line);
                close(pipefd[1]);
                close(pipefd[0]);
                g_signal=S_BASE;
                return (-1);              // error por interrupción
            }
			if (!line || strcmp(line, remove_all_quotes(redir->limiter[i])) == 0)
			{
				free(line);
				break;
			}
			// escribe la línea al pipe
			if (strchr(redir->limiter[i], '\'') != NULL || strchr(redir->limiter[i], '\"') != NULL)
			{
				write(pipefd[1], line, strlen(line));
				write(pipefd[1], "\n", 1);
			}
			else
			{
				expanded = expand_line_heredoc(line);
				write(pipefd[1], expanded, ft_strlen(expanded));
				write(pipefd[1], "\n", 1);
				free(expanded);
			}
			free(line);
		}
		close(pipefd[1]); // cerramos el write end
		redir->heredoc_fds[i] = pipefd[0]; // guardamos el read end
        //g_signal = S_BASE;
        signal_init();
	}
    //set_defaul_signals();
    g_signal = S_BASE;
	// por defecto, el input viene del último heredoc si existen
	redir->input_file = redir->heredoc_fds[redir->heredoc_count - 1];

	return (0);
}
// Procesa recursivamente todos los heredocs del AST antes de ejecutar
int preprocess_heredocs(t_ast *node)
{
    if (!node)
        return 0;

    if (node->type == NODE_COMMAND)
    {
        if (node->cmd && node->cmd->redir && node->cmd->redir->heredoc_count > 0)
        {
            // activa el modo heredoc
            
            if (process_all_heredocs(node->cmd->redir) == -1)
            {
                //g_signal = S_HEREDOC_END;
                return (-1); // interrumpido
            }
            //g_signal = S_BASE;
        }
    }
    else if (node->type == NODE_PIPE || node->type == NODE_AND ||
             node->type == NODE_OR)
    {
        // procesa heredocs en ambos lados
        if (preprocess_heredocs(node->left) == -1)
            return -1;
        if (preprocess_heredocs(node->right) == -1)
            return -1;
    }
    else if (node->type == NODE_SUBSHELL)
    {
        if (preprocess_heredocs(node->left) == -1)
            return -1;
    }
    return 0;
}

/**
 * funcion para procesar los heredocs antes de
 * iniciar la ejecucion v1
 */
// int preprocess_heredocs(t_ast *node)
// {
//     if (!node) return 0;

//     if (node->type == NODE_COMMAND && node->cmd && node->cmd->redir)
//     {
//         if (process_all_heredocs(node->cmd->redir) == -1)
//             return -1;
//     }
//     else if (node->type == NODE_PIPE || node->type == NODE_AND || node->type == NODE_OR)
//     {
//         if (preprocess_heredocs(node->left) == -1)
//             return -1;
//         if (preprocess_heredocs(node->right) == -1)
//             return -1;
//     }
//     return 0;
// }


/**
 * processallheredocs version de roger
 */
// int	process_all_heredocs(t_redir *redir)
// {
// 	int	i;

// 	if (!redir || redir->heredoc_count == 0)
// 		return (0);
// 	i = 0;
// 	while (i < redir->heredoc_count)
// 	{
// 		if (process_single_heredoc(redir->limiter[i], \
// 			&redir->heredoc_fds[i]) == -1)
// 			{
// 				// ft_printf("\nentro\n");
// 				return (-1);
				
// 			}
// 		i++;
// 	}
// 	redir->input_file = redir->heredoc_fds[redir->heredoc_count - 1];
// 	return (0);
// }
