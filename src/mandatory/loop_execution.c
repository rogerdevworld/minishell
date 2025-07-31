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
