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
#include "../../include/minishell.h"

// -- Función para convertir el tipo de token en una cadena -- //
char	*token_type_to_string(int type)
{
	if (type == TOKEN_WORD)
		return ("WORD");
	else if (type == TOKEN_PIPE)
		return ("PIPE");
	else if (type == TOKEN_REDIR_IN)
		return ("REDIR_IN");
	else if (type == TOKEN_REDIR_OUT)
		return ("REDIR_OUT");
	else if (type == TOKEN_APPEND)
		return ("APPEND");
	else if (type == TOKEN_HEREDOC)
		return ("HEREDOC");
	else if (type == TOKEN_AND)
		return ("AND");
	else if (type == TOKEN_OR)
		return ("OR");
	else if (type == TOKEN_OPEN_PAREN)
		return ("OPEN_PAREN");
	else if (type == TOKEN_CLOSE_PAREN)
		return ("CLOSE_PAREN");
	else if (type == TOKEN_INVALID)
		return ("INVALID");
	else
		return ("UNKNOWN");
}

// -- Función para imprimir tokens -- //
void	print_tokens(t_token *tokens)
{
	while (tokens)
	{
		ft_putstr_fd("Token Type -> ", 1);
		ft_putstr_fd(token_type_to_string(tokens->type), 1);
		ft_putstr_fd(" -> ", 1);
		ft_putstr_fd(tokens->value, 1);
		ft_putstr_fd("\n", 1);
		tokens = tokens->next;
	}
}
