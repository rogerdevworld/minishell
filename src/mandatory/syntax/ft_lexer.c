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
#include "../../../include/minishell.h"

/**
 * Initializes a new `t_token` structure with a given string value.
 * It allocates memory for the token and its value,
	then determines the token's type
 * based on the value (e.g., AND, OR, PIPE, REDIRECTION, WORD, PARENTHESES,
	etc.).
 * Returns a pointer to the newly created token,
	or NULL if memory allocation fails.
 */
t_token	*init_lexer(char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(value);
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	token->quote_type = '\0';
	token->next = NULL;
	token->type = set_token_type(value);
	return (token);
}

/**
 * Adds a new token to the end of a linked list of tokens.
 * If the list is empty, the new token becomes the head.
 */
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

/**

 * The main lexer function that takes an input string and converts 
	it into a linked list of tokens.
 * It iterates through the input,
	identifying and extracting operators and words (including quoted sections).
 * Returns the head of the linked list of tokens.
 */
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
		free(token_val);
	}
	return (tokens);
}
