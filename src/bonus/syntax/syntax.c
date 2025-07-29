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
#include "../../../include/minishell.h"

int	ft_msg_syntax(char *error, char *arg)
{
	if (!arg)
		ft_putstr_fd(error, 2);
	if (arg)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(error, 2);
		ft_putstr_fd("`", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("'", 2);
	}
	ft_putstr_fd("\n", 2);
	return (1);
}

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
			ft_msg_syntax("Syntax error: invalid token ", tokens->value);
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
	// Initial check: An operator cannot be the first token
	if (tokens->type == TOKEN_PIPE || tokens->type == TOKEN_AND
		|| tokens->type == TOKEN_BG || tokens->type == TOKEN_OR)
		return (ft_msg_syntax("syntax error near unexpected token ", tokens->value), 2);
	while (tokens)
	{
		// Check for consecutive operators
		if (prev && (prev->type == TOKEN_PIPE || prev->type == TOKEN_AND
				|| prev->type == TOKEN_OR || prev->type == TOKEN_BG))
			// Added TOKEN_BG here
		{
			if (tokens->type == TOKEN_PIPE || tokens->type == TOKEN_AND
				|| tokens->type == TOKEN_OR || tokens->type == TOKEN_BG)
				// Added TOKEN_BG here
				return (ft_msg_syntax("syntax error near unexpected token ", tokens->value),
					2);
		}
		prev = tokens;
		tokens = tokens->next;
	}
	// Final check: An operator cannot be the last token
	if (prev && (prev->type == TOKEN_PIPE || prev->type == TOKEN_AND
			|| prev->type == TOKEN_OR || prev->type == TOKEN_BG))
		// Added TOKEN_BG here
		return (ft_msg_syntax("syntax error near unexpected token ", prev->value), 2);
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
				ft_msg_syntax("syntax error near unexpected token ", tokens->value);
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
				ft_msg_syntax("Syntax error: unexpected ')'\n", NULL);
				return (-1); // Error por cierre inesperado
			}
		}
		tokens = tokens->next;
	}
	return (count); // Si es > 0, faltan cierres
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