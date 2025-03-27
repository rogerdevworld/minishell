#ifndef MINISHELL_LEXER_H
# define MINISHELL_LEXER_H

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

#endif