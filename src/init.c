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

/* t_minishell	*init_minishell(char *line, char **envp)
{
	t_token		*tokens;
	t_command	*cmd;
	t_ast_node	*ast_root;
	t_executor	*exec;
	t_myenv		*myenv;
	t_minishell	*minishell;

	tokens = NULL;
	myenv = ft_myenv(envp);
	exec = init_exec(myenv);
	tokens = lexer(line);
	cmd = parse_tokens(tokens, envp);
	ast_root = build_ast(cmd);
	minishell = malloc(sizeof(t_minishell));
	if (!minishell)
		return (NULL);
	minishell->tokens = tokens;
	minishell->cmd = cmd;
	minishell->ast_tree = ast_root;
	minishell->executor = exec;
	minishell->env = exec->myenv;
	minishell->exit = 0;
	return (minishell);
} */

t_minishell	*init_minishell(t_ast_node *ast, t_token *tokens, t_command *cmd,
		t_executor *exec)
{
	t_minishell	*minishell;

	minishell = malloc(sizeof(t_minishell));
	if (!minishell)
		return (NULL);
	minishell->tokens = tokens;
	minishell->cmd = cmd;
	minishell->ast_tree = ast;
	minishell->executor = exec;
	minishell->env = exec->myenv;
	minishell->exit = 0;
	return (minishell);
}

/**
 * aantiguo init_minishell usado para inicializar la estructura t_minishell
 */
/*
t_minishell	*init_minishell(t_myenv *env, t_ast_node *ast_tree, t_token *tokens,
		t_command *cmd,	t_executor *executor)
{
	t_minishell	*minishell;

	minishell = malloc(sizeof(t_minishell));
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
	minishell->exit = executor->status;
	return (minishell);
}
 */
