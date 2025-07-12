/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:56:13 by rmarrero          #+#    #+#             */
/*   Updated: 2025/03/27 11:57:56 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef LEXER_H
# define LEXER_H

// -- lexer.h -- //
enum
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_OPEN_PAREN,
	TOKEN_CLOSE_PAREN,
	TOKEN_INVALID
};

typedef struct s_token
{
	char			*value;
	int				type;
	char			quote_type; // para manejar comillas en lexer
	struct s_token	*next;
}					t_token;

typedef struct s_word
{
	char			*value;
	char			quote_type;
}					t_word;

t_token				*lexer(char *str);
t_token				*init_lexer(char *token);
// t_token				*init_lexer(char *token, char quote_type); 
void				add_back(t_token **tokens, t_token *token);
int					validate_syntax(t_token *tokens);

#endif