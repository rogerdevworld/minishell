/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_syntax_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 20:57:47 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/16 20:57:52 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../include/minishell.h"

/**
 * Initializes a new `t_command` structure.
 * Allocates memory for the command itself, its arguments array,
	and its redirection structure.
 * Initializes the redirection structure using `init_redir`.
 * Returns a pointer to the newly created command,
	or NULL if memory allocation fails.
 */
t_command	*init_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = ft_calloc(64, sizeof(char *));
	cmd->path = NULL;
	cmd->redir = ft_calloc(1, sizeof(t_redir));
	if (!cmd->redir)
	{
		free(cmd->args);
		free(cmd);
		return (NULL);
	}
	init_redir(cmd->redir);
	return (cmd);
}

/**
 * Initializes a new Abstract Syntax Tree (AST) node.
 * Allocates memory for the node and sets its type and associated command.
 * The left and right child pointers are initialized to NULL.
 * Returns a pointer to the newly created AST node,
	or NULL if memory allocation fails.
 */
t_ast	*init_ast_node(t_node_type type, t_command *cmd)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = type;
	node->cmd = cmd;
	node->left = NULL;
	node->right = NULL;
	return (node);
}
