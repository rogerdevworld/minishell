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
#include "../../../include/minishell.h"

/**
 * Parses a shell expression, handling logical AND (`&&`), OR (`||`),
	and background (`&`) operators.

 * It builds an Abstract Syntax Tree (AST) by recursively parsing
	pipelines and combining them with
 * logical operators based on their precedence.
 * Returns the root of the AST for the parsed expression.
 */
t_ast	*parse_expression(t_token **tokens, char **envp)
{
	t_ast		*left;
	t_ast		*new;
	t_node_type	type;

	left = parse_pipeline(tokens, envp);
	while (*tokens && ((*tokens)->type == TOKEN_AND
			|| (*tokens)->type == TOKEN_OR || (*tokens)->type == TOKEN_BG))
	{
		if ((*tokens)->type == TOKEN_AND)
			type = NODE_AND;
		else if ((*tokens)->type == TOKEN_OR)
			type = NODE_OR;
		else
			type = NODE_BG;
		next_token(tokens);
		new = init_ast_node(type, NULL);
		new->left = left;
		new->right = parse_pipeline(tokens, envp);
		left = new;
	}
	return (left);
}

/**
 * Executes a command in the background.
 * It forks a new process, and in the child process, it redirects
	standard input to /dev/null
 * and then executes the command associated with the left child of the AST node.
 * The parent process does not wait for the background command to
	complete and continues execution,
 * optionally executing a right-hand side command if present.
 * Returns the status of the right-hand side command if executed,
	or 0 on successful backgrounding.
 */
int	execute_bg(t_ast *node, t_myenv *myenv, t_minishell *minishell)
{
	pid_t	pid;
	int		status;

	g_signal = S_CMD;
	pid = fork();
	status = 0;
	if (pid == 0)
	{
		set_defaul_signals();
		close(STDIN_FILENO);
		open("/dev/null", O_RDONLY);
		exit(execute_ast(node->left, myenv, minishell, status));
	}
	else if (pid > 0)
	{
		if (node->right)
			status = execute_ast(node->right, myenv, minishell, status);
		else
		{
			perror("fork");
			status = 1;
		}
	}
	return (status);
}
