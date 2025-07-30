/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:56:13 by rmarrero          #+#    #+#             */
/*   Updated: 2025/04/09 22:22:59 by xviladri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/minishell.h"

/**
 * @brief Checks if an AST node represents a command composed solely 
 	of a heredoc.
 * This function determines if a given Abstract Syntax Tree (AST) node
 * corresponds to a command that *only* contains a heredoc redirection,
 * without any command arguments. This is useful for specific handling of
 * heredoc-only commands, such as when they appear as the sole component
 * of a pipeline or subshell, where their exit status might need to be
 * handled differently.
 *
 * @param ast A pointer to the AST node to check.
 * @return Returns 1 if the node is a command consisting only of a heredoc,
 * otherwise returns 0.
 */
int	is_heredoc_only(t_ast *ast)
{
	if (!ast || ast->type != NODE_COMMAND)
		return (0);
	if (!ast->cmd)
		return (0);
	if (ast->cmd->args && ast->cmd->args[0])
		return (0);
	if (ast->cmd->redir && ast->cmd->redir->heredoc_count > 0)
		return (1);
	return (0);
}

/**
 * @brief Parses the tokens into an Abstract Syntax Tree (AST) and 
	executes the command.
 * This function takes the processed tokens, builds an AST, handles any
 * heredoc redirections, initializes the minishell execution context, and
 * then executes the command represented by the AST. It also manages
 * the exit status and resource cleanup.
 *
 * @param data A pointer to a `t_loop_data` structure containing tokens,
 * full line, and where the AST, minishell context, and status will be stored.
 * @param myenv A pointer to the custom environment variables.
 */
void	parse_and_execute(t_loop_data *data, t_myenv *myenv)
{
	t_token	*tmp;

	tmp = data->tokens;
	data->ast = parse_expression(&data->tokens, myenv->env);
	free_tokens(tmp);
	if (preprocess_heredocs(data->ast, data->status) == -1)
	{
		data->status = 130;
		free_ast(data->ast);
		free(data->full_line);
		data->minishell = NULL;
		return ;
	}
	else
	{
		data->minishell = init_minishell(data->ast, myenv, data->full_line);
		if (is_heredoc_only(data->ast))
			data->status = 0;
		else if (g_signal != S_CANCEL_EXEC)
			data->status = execute_ast(data->ast, myenv,
					data->minishell, data->status);
	}
}
