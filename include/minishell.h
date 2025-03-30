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

// -- system.h -- //

// -- system libs -- //
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

// -- lexer.h -- //
typedef enum
{
	TOKEN_COMMAND,
	TOKEN_OPERATOR,
	TOKEN_SEPARATOR,
	TOKEN_REDIRECTION,
	TOKEN_ARGUMENT,
	TOKEN_END
}						TokenType;

typedef struct s_token
{
	char				*value;
	TokenType			type;
	struct s_token		*next;
}						t_token;

t_token					*lexer(char *str);
t_token					*init_lexer(char *token);
t_token					*get_last_token(t_token *token);
void					add_back(t_token **tokens, t_token *token);

// -- minishell.h -- //

// -- internal strcts -- //
typedef struct s_token	t_token;

void					print_tokens(t_token *tokens);
char					*token_type_to_string(TokenType type);
void					ft_exit(int status, char *msg);
void					main_loop(char *delimiter, int *p_fd);
void					ft_here_doc(char *delimiter);

// -- free -- //
void					free_split(char **split);
#endif
