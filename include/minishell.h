/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:36:11 by rmarrero          #+#    #+#             */
/*   Updated: 2025/03/27 17:36:15 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// -- internal libs -- //
# include "../src/libft/libft.h"
# include "lexer.h"
# include "parser.h"
# include "builtins.h"
# include "design.h"
# include "executor.h"
# include "executor.h"
# include "signals.h"
# include "design.h"

// -- system.h -- //

// -- system libs -- //
# include <fcntl.h>
# include <limits.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>
#include <signal.h>
#include <limits.h>

// -- readline -- //
# include <readline/history.h>
# include <readline/readline.h>

// -- minishell.h -- //

// -- internal strcts -- //
typedef struct s_minishell
{
	char		**envp;
	t_token		*tokens;
	t_command	*cmd;
}				t_minishell;

t_minishell	*init_minishell(char **envp, t_token *tokens, t_command *cmd);


// -- main loop -- //
void			main_loop(char **envp);

// -- exit.h -- //
void			ft_exit(char *msg);

#endif