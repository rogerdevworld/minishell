/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 20:16:29 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/16 20:16:34 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../include/minishell.h"

/**
 * Recursively executes an Abstract Syntax Tree (AST) node.
 * It dispatches to specific execution functions based on the node's type.
 * Supported types include commands, pipes, logical AND/OR, subshells,
	and background commands.
 * Returns the exit status of the executed node.
 */
int	execute_ast(t_ast *node, t_myenv *myenv, t_minishell *minishell, int status)
{
	if (!node)
		return (0);
	if (node->type == NODE_COMMAND)
		return (execute_command(node->cmd, minishell, status));
	else if (node->type == NODE_PIPE)
		return (execute_pipe(node, myenv, minishell));
	else if (node->type == NODE_AND)
		return (execute_and(node, myenv, minishell));
	else if (node->type == NODE_OR)
		return (execute_or(node, myenv, minishell));
	else if (node->type == NODE_SUBSHELL)
		return (execute_subshell(node, myenv, minishell));
	else if (node->type == NODE_BG)
		return (execute_bg(node, myenv, minishell));
	return (1);
}
