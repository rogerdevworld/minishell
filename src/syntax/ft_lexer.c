/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 20:41:09 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/16 20:41:15 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

t_token	*init_lexer(char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(value);
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
	t_token	*tmp;

	if (!new_token)
		return ;
	if (!*tokens)
	{
		*tokens = new_token;
		return ;
	}
	tmp = *tokens;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_token;
}

t_token	*lexer(char *input)
{
	t_token	*tokens;
	char	*token_val;
	int		i;
	int		len;

	tokens = NULL;
	token_val = NULL;
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
	free(token_val);
	return (tokens);
}

void	shift_empty_tokens(t_token **head)
{
	t_token	*tmp;
	t_token	*prev;
	t_token	*to_delete;

	tmp = *head;
	prev = NULL;
	while (tmp)
	{
		if (!tmp->value || tmp->value[0] == '\0')
		{
			to_delete = tmp;
			if (prev == NULL)
			{
				*head = tmp->next;
				tmp = *head;
			}
			else
			{
				prev->next = tmp->next;
				tmp = tmp->next;
			}
			if (to_delete->value)
				free(to_delete->value);
			free(to_delete);
		}
		else
		{
			prev = tmp;
			tmp = tmp->next;
		}
	}
}
