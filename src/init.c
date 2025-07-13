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
#include "../include/minishell.h"

t_minishell	*init_minishell(t_ast *ast, t_token *tokens, t_command *cmd,
		t_myenv *env)
{
	t_minishell *minishell;

	minishell = malloc(sizeof(t_minishell));
	if (!minishell)
		return (NULL);
	minishell->tokens = tokens;
	minishell->cmd = cmd;
	minishell->ast = ast;
	minishell->env = env;
	minishell->exit = 0;
	return (minishell);
}