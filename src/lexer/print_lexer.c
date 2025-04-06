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
	if (type == TOKEN_COMMAND)
		return ("COMANDO");
	else if (type == TOKEN_OPERATOR)
		return ("OPERADOR");
	else if (type == TOKEN_ARGUMENT)
		return ("ARGUMENT");
	else if (type == TOKEN_REDIRECTION)
		return ("REDIRECCION");
	else
		return ("DESCONOCIDO");
}

void	print_tokens(t_token *tokens)
{
	while (tokens)
	{
		ft_putstr_fd("Token Type -> ", 1);
		ft_putstr_fd(token_type_to_string(tokens->type), 1);
		ft_putstr_fd("-> ", 2);
		ft_putstr_fd(tokens->value, 1);
		ft_putstr_fd("\n", 1);
		tokens = tokens->next;
	}
}
