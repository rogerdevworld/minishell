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
#include "../include/minishell.h"

// -- main funtion for lexer -- //
t_token	*lexer(char *token)
{
	t_token	*tokens;
	int		i;
	char	**tmp;

	tokens = NULL;
	i = 0;
	tmp = ft_split(token, ' ');
	if (!tmp)
		return (NULL);
	while (tmp[i])
	{
		add_back(&tokens, init_lexer(tmp[i]));
		i++;
	}
	free_split(tmp);
	return (tokens);
}

// -- malloc t_token -- //
t_token	*init_lexer(char *token)
{
	t_token	*result;

	result = (t_token *)malloc(sizeof(t_token));
	if (!result)
		return (NULL);
	result->value = ft_strdup(token);
	if (!result->value)
	{
		free(result);
		return (NULL);
	}
	if (ft_strcmp(token, "&&") == 0)
		result->type = TOKEN_OPERATOR;
	else if (ft_strcmp(token, "||") == 0)
		result->type = TOKEN_OPERATOR;
	else if (ft_strcmp(token, ";") == 0)
		result->type = TOKEN_SEPARATOR;
	else if (token[0] == '>' || token[0] == '<')
		result->type = TOKEN_REDIRECTION;
	else
		result->type = TOKEN_COMMAND;
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

// -- Función para convertir el tipo de token en una cadena -- //
char	*token_type_to_string(TokenType type)
{
	if (type == TOKEN_COMMAND)
		return ("COMANDO");
	else if (type == TOKEN_OPERATOR)
		return ("OPERADOR");
	else if (type == TOKEN_SEPARATOR)
		return ("SEPARADOR");
	else if (type == TOKEN_REDIRECTION)
		return ("REDIRECCION");
	else
		return ("DESCONOCIDO");
}
