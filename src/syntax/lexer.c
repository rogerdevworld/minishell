#include "../../include/minishell.h"

t_token	*init_lexer(char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = value;
	token->next = NULL;
	if (!value)
		token->type = TOKEN_INVALID;
	else if (ft_strcmp(value, "&&") == 0)
		token->type = TOKEN_AND;
	else if (ft_strncmp(value, "||", 2) == 0)
		token->type = TOKEN_OR;
	else if (ft_strncmp(value, "|", 2) == 0)
		token->type = TOKEN_PIPE;
	else if (ft_strncmp(value, "<<", 2) == 0)
		token->type = TOKEN_HEREDOC;
	else if (ft_strcmp(value, ">") == 0)
		token->type = TOKEN_REDIR_OUT;
	else if (ft_strcmp(value, ">>") == 0)
		token->type = TOKEN_APPEND;
	else if (ft_strncmp(value, "<", 1) == 0)
		token->type = TOKEN_REDIR_IN;
	else if (ft_strcmp(value, "(") == 0)
		token->type = TOKEN_OPEN_PAREN;
	else if (ft_strcmp(value, ")") == 0)
		token->type = TOKEN_CLOSE_PAREN;
	else
		token->type = TOKEN_WORD;
	return (token);
}

void	add_back(t_token **tokens, t_token *new_token)
{
	t_token	*curr;

	if (!new_token)
		return ;
	if (!*tokens)
	{
		*tokens = new_token;
		return ;
	}
	curr = *tokens;
	while (curr->next)
		curr = curr->next;
	curr->next = new_token;
}

t_token	*lexer(char *input)
{
	t_token	*tokens;
	char	*token_val;
	int		i;
	int		len;

	tokens = NULL;
	i = 0;
	len = 0;
	while (input[i])
	{
		while (ft_isspace(input[i]))
			i++;
		if (!input[i])
			break ;
		if (is_operator(&input[i]))
			len = read_operator(&input[i], &token_val);
		else
			len = read_word(&input[i], &token_val);
		add_back(&tokens, init_lexer(token_val));
		i += len;
	}
	return (tokens);
}
