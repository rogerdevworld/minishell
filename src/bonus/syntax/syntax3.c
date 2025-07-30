/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:22:35 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/15 14:22:38 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../include/minishell.h"

static int	check_consecutive_ops(t_token *tokens, t_token **last_token)
{
	t_token	*prev;

	prev = NULL;
	while (tokens)
	{
		if (prev && (prev->type == TOKEN_PIPE || prev->type == TOKEN_AND
				|| prev->type == TOKEN_OR || prev->type == TOKEN_BG))
		{
			if (tokens->type == TOKEN_PIPE || tokens->type == TOKEN_AND
				|| tokens->type == TOKEN_OR || tokens->type == TOKEN_BG)
				return (ft_msg_syntax("syntax error near unexpected token ",
						tokens->value), 2);
		}
		prev = tokens;
		tokens = tokens->next;
	}
	*last_token = prev;
	return (0);
}

int	check_operator_positions(t_token *tokens)
{
	t_token	*last_token;

	if (!tokens)
		return (0);
	if (tokens->type == TOKEN_PIPE || tokens->type == TOKEN_AND
		|| tokens->type == TOKEN_BG || tokens->type == TOKEN_OR)
		return (ft_msg_syntax("syntax error near unexpected token ",
				tokens->value), 2);
	if (check_consecutive_ops(tokens, &last_token) != 0)
		return (2);
	if (last_token && (last_token->type == TOKEN_PIPE
			|| last_token->type == TOKEN_AND
			|| last_token->type == TOKEN_OR
			|| last_token->type == TOKEN_BG))
		return (ft_msg_syntax("syntax error near unexpected token ",
				last_token->value), 2);
	return (0);
}

/*
 * validate_syntax:
 * Función que ejecuta todas las validaciones anteriores.
 * Devuelve 1 si encuentra algún error de sintaxis, 0 si no.
 */
int	validate_syntax(t_token *tokens)
{
	if (check_invalid_tokens(tokens))
		return (2);
	if (check_operator_positions(tokens))
		return (2);
	if (check_redirection_args(tokens))
		return (2);
	if (check_parentheses_balance(tokens))
		return (2);
	return (0);
}
