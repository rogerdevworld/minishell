/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:56:13 by rmarrero          #+#    #+#             */
/*   Updated: 2025/04/09 22:22:59 by xviladri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// --- System Libraries ---
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <signal.h>
# include <sys/stat.h>
# include <dirent.h>
# include <sys/ioctl.h>
# include <limits.h>
# include <ctype.h>

// --- Readline Library ---
# include <readline/readline.h>
# include <readline/history.h>

// --- Internal Libraries (Order is critical here) ---
# include "../src/libft/libft.h"
# include "env.h"
# include "syntax.h" // Must be before other headers that use its structs
# include "builtins.h"
# include "exec.h"
# include "redirections.h"
# include "signals.h"
# include "utils.h"

// -- Global Signal Variable --
extern int	g_signal;

// -- Shell Exit Codes --
# define EXIT_SUCCESS 0
# define EXIT_GENERAL_ERROR 1
# define EXIT_MISUSE_BUILTIN 2
# define EXIT_CMD_NOT_FOUND 127
# define EXIT_NOT_EXECUTABLE 126
# define EXIT_SIGINT 130
# define EXIT_SIGQUIT 131
# ifndef PATH_MAX
#  define PATH_MAX 1024
# endif

// -- Colors for Prompt --
# define RESET "\001\033[0m\002"
# define GREEN "\001\033[0;32m\002"

// -- Main Minishell Structure --
typedef struct s_minishell
{
	t_token		*tokens;
	t_ast		*ast;
	t_myenv		*myenv;
	char		*full_line;
	int			exit;
}				t_minishell;

typedef struct s_loop_data
{
	char		*full_line;
	t_token		*tokens;
	t_ast		*ast;
	t_minishell	*minishell;
	int			status;
}				t_loop_data;

// -- Initialization --
t_minishell		*init_minishell(t_ast *ast, t_myenv *myenv, char *full_line);

// -- Main Loop (main_loop.c) --
void			main_loop(t_myenv *myenv);

// -- Loop Logic (loop_*.c) --
int				read_and_prepare_line(t_loop_data *data);
int				process_and_validate_tokens(t_loop_data *data, t_myenv *myenv);
void			parse_and_execute(t_loop_data *data, t_myenv *myenv);
int				is_heredoc_only(t_ast *ast);

#endif