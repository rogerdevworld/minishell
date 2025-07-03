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

// -- internal libs -- //
# include "../src/libft/libft.h"
# include "env.h"
# include "lexer.h"
# include "parser.h"
# include "builtins.h"
# include "design.h"
# include "executor.h"
# include "redirections.h"
# include "signals.h"
# include "syntax.h"

// -- system.h -- //

// -- system libs -- //
# include <dirent.h>
# include <dirent.h>
# include <fcntl.h>
# include <limits.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

// -- readline -- //
# include <readline/history.h>
# include <readline/readline.h>

// -- minishell.h -- //

// -- internal strcts -- //
typedef struct s_minishell
{
	t_token		*tokens;
	t_command	*cmd;
	t_myenv		*env;
	t_executor	*executor;
	int exit;
}				t_minishell;

t_minishell	*init_minishell(t_myenv *env, t_token *tokens, t_command *cmd,
	t_executor *executor);

// -- main loop -- //
void			main_loop(char **envp);

void			ft_wildcards(char ***args);
// -- free -- //
void			free_tokens(t_token *tokens);
void			free_command_list(t_command *cmd);
// -- exit.h -- //
void			ft_exit(char *msg);

#endif
