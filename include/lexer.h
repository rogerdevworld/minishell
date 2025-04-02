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
	TOKEN_COMMAND,
	TOKEN_OPERATOR,
	TOKEN_SEPARATOR,
	TOKEN_REDIRECTION,
	TOKEN_ARGUMENT,
	TOKEN_END
};

typedef struct s_token
{
	char			*value;
	int				type;
	struct s_token	*next;
}					t_token;

t_token				*lexer(char *str);
t_token				*init_lexer(char *token);
t_token				*get_last_token(t_token *token);
void				add_back(t_token **tokens, t_token *token);
void				print_tokens(t_token *tokens);
char				*token_type_to_string(int type);

// -- ft_split.h -- //
char				**ft_split_quotes(char const *s, char c);
void				free_split(char **split);

#endif