/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:22:35 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/15 14:22:38 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

/*
 * check_invalid_tokens:
 * Recorre la lista de tokens y detecta si existe algún token
 * marcado como inválido.
 * Si encuentra uno, imprime un mensaje de error con el token y devuelve 1.
 * Si no hay tokens inválidos, devuelve 0.
 */
int	check_invalid_tokens(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->type == TOKEN_INVALID)
		{
			ft_printf("Syntax error: invalid token '%s'\n", tokens->value);
			return (1);
		}
		tokens = tokens->next;
	}
	return (0);
}

/*
 * check_operator_positions:
 * Valida la posición de operadores lógicos y pipes.
 * No permite que la línea empiece o termine con operadores |, &&, ||.
 * Tampoco permite operadores consecutivos sin comandos entre ellos.
 * Si encuentra error,
	imprime mensaje y devuelve 1; si todo está correcto devuelve 0.
 */
int	check_operator_positions(t_token *tokens)
{
	t_token	*prev;

	prev = NULL;
	if (!tokens)
		return (0);
	if (tokens->type == TOKEN_PIPE || tokens->type == TOKEN_AND
		|| tokens->type == TOKEN_OR)
		return (ft_printf("Syntax error: unexpected token '%s' at start\n",
				tokens->value), 2);
	while (tokens)
	{
		if (prev && (prev->type == TOKEN_PIPE || prev->type == TOKEN_AND
				|| prev->type == TOKEN_OR))
		{
			if (tokens->type == TOKEN_PIPE || tokens->type == TOKEN_AND
				|| tokens->type == TOKEN_OR)
				return (ft_printf("Syntax error: unexpected token '%s' after '%s'\n",
						tokens->value, prev->value), 2);
		}
		prev = tokens;
		tokens = tokens->next;
	}
	if (prev && (prev->type == TOKEN_PIPE || prev->type == TOKEN_AND
			|| prev->type == TOKEN_OR))
		return (ft_printf("Syntax error: unexpected token '%s' at end\n",
				prev->value), 2);
	return (0);
}

/*
 * check_redirection_args:
 * Comprueba que cada token de redirección (<, >, >>,
	<<) esté seguido por un token válido
 * de tipo WORD (nombre de archivo).
 * Si falta el argumento después de una redirección, imprime error y devuelve 1.
 * Devuelve 0 si todas las redirecciones están bien seguidas.
 */
int	check_redirection_args(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->type == TOKEN_REDIR_IN || tokens->type == TOKEN_REDIR_OUT
			|| tokens->type == TOKEN_APPEND || tokens->type == TOKEN_HEREDOC)
		{
			if (!tokens->next || tokens->next->type != TOKEN_WORD)
			{
				ft_printf("Syntax error: missing filename after '%s'\n",
					tokens->value);
				return (1);
			}
		}
		tokens = tokens->next;
	}
	return (0);
}

/*
 * check_parentheses_balance:
 * Recorre la lista de tokens contando paréntesis abiertos y cerrados.
 * Detecta paréntesis cerrados sin su correspondiente abierto y viceversa.
 * Imprime error y devuelve 1 si hay desbalance; devuelve 0 si
 * están balanceados.
 */
int	check_parentheses_balance(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens)
	{
		if (tokens->type == TOKEN_OPEN_PAREN)
			count++;
		else if (tokens->type == TOKEN_CLOSE_PAREN)
		{
			count--;
			if (count < 0)
			{
				ft_printf("Syntax error: unexpected ')'\n");
				return (1);
			}
		}
		tokens = tokens->next;
	}
	if (count != 0)
	{
		ft_printf("Syntax error: unclosed '('\n");
		return (1);
	}
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
