/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_syntax_subshell.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 20:58:45 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/16 20:58:49 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

// Paréntesis o comando base
t_ast	*parse_factor(t_token **tokens, char **envp)
{
	t_ast	*subtree;
	t_ast	*node;

	if (!*tokens)
		return (NULL);
	if ((*tokens)->type == TOKEN_OPEN_PAREN)
	{
		next_token(tokens);
		subtree = parse_expression(tokens, envp);
		if (*tokens && (*tokens)->type == TOKEN_CLOSE_PAREN)
			next_token(tokens);
		node = init_ast_node(NODE_SUBSHELL, NULL);
		node->left = subtree;
		return (node);
	}
	else
		return (parse_simple_command(tokens, envp));
}
