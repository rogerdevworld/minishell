/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:36:11 by rmarrero          #+#    #+#             */
/*   Updated: 2025/04/09 22:22:45 by xviladri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef MINISHELL_H
# define MINISHELL_H

// -- variable global signal -- //
extern int	g_signal;

// -- internal libs -- //
# include "../src/utils/libft/libft.h"
# include "syntax.h"
# include "env.h"
# include "builtins.h"
# include "design.h"
# include "exec.h"
# include "redirections.h"
# include "signals.h"

# include "utils.h"
// -- system.h -- //

// -- system libs -- //
# include <ctype.h>
# include <dirent.h>
# include <fcntl.h>
# include <limits.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

// -- readline -- //
# include <readline/history.h>
# include <readline/readline.h>

// -- minishell exit code -- //
// Comando ejecutado con éxito
# define EXIT_SUCCESS 0

// Errores comunes
# define EXIT_GENERAL_ERROR 1 // Error genérico
# define EXIT_MISUSE_BUILTIN 2 // Uso incorrecto de un comando builtin
# define EXIT_CMD_NOT_FOUND 127 // Comando no encontrado
# define EXIT_NOT_EXECUTABLE 126 // Comando no ejecutable

// Señales comunes
# define EXIT_SIGINT 130 // Ctrl+C
# define EXIT_SIGQUIT 131 // Ctrl+\ (quit)

// Código especial al usar 'exit' incorrectamente
# define EXIT_INVALID_EXIT 128
# ifndef PATH_MAX
#  define PATH_MAX 1024
# endif
// -- minishell.h -- //
// -- internal strcts -- //
typedef struct s_minishell
{
	t_token	*tokens;
	t_ast	*ast;
	t_myenv	*myenv;
	int		exit;
}			t_minishell;

t_minishell	*init_minishell(t_ast *ast, t_token *tokens, t_myenv *myenv);

// -- main loop -- //
void		main_loop(t_myenv *myenv);

void		ft_wildcards(char ***args);
// -- free -- //
void		free_tokens(t_token *tokens);
// void			free_command_list(t_command *cmd);
//  -- exit.h -- //
void		ft_exit(char *msg);

#endif
