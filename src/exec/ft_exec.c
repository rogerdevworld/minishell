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
#include "../../include/minishell.h"

int	execute_ast(t_ast *node, char **envp, t_myenv *myenv,
		t_minishell *minishell, int status)
{
	if (!node)
		return (0);
	if (node->type == NODE_COMMAND)
		return (execute_command(node->cmd, envp, myenv, minishell, status));
	else if (node->type == NODE_PIPE)
		return (execute_pipe(node, envp, myenv, minishell));
	else if (node->type == NODE_AND)
		return (execute_and(node, envp, myenv, minishell));
	else if (node->type == NODE_OR)
		return (execute_or(node, envp, myenv, minishell));
	else if (node->type == NODE_SUBSHELL)
		return (execute_subshell(node, envp, myenv, minishell));
	return (1);
}
