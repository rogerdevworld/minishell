/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_syntax_pipes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 20:58:21 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/16 20:58:27 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../include/minishell.h"

/**
 * Parses a sequence of piped commands.
 * It builds an Abstract Syntax Tree (AST) where commands are chained together
 * by `|` (pipe) operators. It recursively parses individual command "factors"
 * and connects them with pipe nodes.
 * Returns the root of the AST representing the pipeline.
 */
t_ast	*parse_pipeline(t_token **tokens, char **envp)
{
	t_ast	*left;
	t_ast	*right;
	t_ast	*new_node;

	left = parse_factor(tokens, envp);
	while (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		next_token(tokens);
		right = parse_factor(tokens, envp);
		new_node = init_ast_node(NODE_PIPE, NULL);
		new_node->left = left;
		new_node->right = right;
		left = new_node;
	}
	return (left);
}
