/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:56:13 by rmarrero          #+#    #+#             */
/*   Updated: 2025/03/27 11:57:56 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/lexer.h"

// -- main funtion for lexer -- //
void	*lexer(char *token)
{
	t_token	*tokens;
	int		i;
	char	**tmp;

	tokens = NULL;
	i = 0;
	tmp = ft_split(token, ' ');
	if (!tmp)
		return ;
	while (tmp[i])
	{
		add_back(&tokens, init_lexer(token[i]));
		i++;
	}
	free_split(tmp);
}

// -- malloc t_token -- //
t_token	*init_lexer(char *token)
{
	t_token	result;

	result = (t_token)malloc(sizeof(t_token));
	if (!result)
		return (NULL);
	result->value = ft_strdup(token);
	if (!result->value)
	{
		free(result);
		return (NULL);
	}
	result->type = 0;
	result->next = NULL;
	return (result);
}

// -- get last token -- //
t_token	*get_last_token(t_token *token)
{
	if (!token)
		return (NULL);
	while (token->next)
		token = token->next;
	return (token);
}

// -- anadir al final -- //
void	add_back(t_token **tokens, t_token *token)
{
	t_token	*last_token;

	if (!token || !tokens)
		return ;
	if (!*tokens)
		*tokens = token;
	else
	{
		last_token = get_last_token(*tokens);
		last_token->next = token;
	}
}
