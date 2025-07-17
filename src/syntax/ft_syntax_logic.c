/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_syntax_logic.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 20:58:00 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/16 20:58:07 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

// && y ||
t_ast	*parse_expression(t_token **tokens, char **envp)
{
	t_ast		*left;
	t_ast		*new;
	t_node_type	type;

	left = parse_pipeline(tokens, envp);
	while (*tokens && ((*tokens)->type == TOKEN_AND
			|| (*tokens)->type == TOKEN_OR))
	{
		if ((*tokens)->type == TOKEN_AND)
			type = NODE_AND;
		else
			type = NODE_OR;
		next_token(tokens);
		new = init_ast_node(type, NULL);
		new->left = left;
		new->right = parse_pipeline(tokens, envp);
		left = new;
	}
	return (left);
}
