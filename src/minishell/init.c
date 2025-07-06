/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 21:49:28 by rmarrero          #+#    #+#             */
/*   Updated: 2025/02/22 21:49:33 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

t_minishell	*init_minishell(t_myenv *env, t_ast_node *ast_tree, t_token *tokens, t_command *cmd,
		t_executor *executor)
{
	t_minishell *minishell = malloc(sizeof(t_minishell));
	if (!minishell || !env || !tokens || !cmd || !executor)
	{
		free(minishell);
		return (NULL);
	}
	minishell->env = env;
	minishell->tokens = tokens;
	minishell->cmd = cmd;
	minishell->executor = executor;
	minishell->ast_tree = ast_tree;
	minishell->exit = 0;
	return (minishell);
}

