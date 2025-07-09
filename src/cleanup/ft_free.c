#include "../../include/minishell.h"

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		if (tmp->value)
			free(tmp->value);
		free(tmp);
	}
}

void	free_command(t_command *cmd)
{
	if (!cmd)
		return ;
	if (cmd->args)
		free_split(cmd->args);
	free(cmd->path);
	free(cmd->limiter);
	free(cmd);
}

void	free_command_list(t_command *cmd)
{
	t_command	*tmp;

	while (cmd)
	{
		tmp = cmd;
		cmd = cmd->next; // si `t_command` tiene una lista enlazada
		free_command(tmp);
	}
}

void	free_ast_node(t_ast_node *node)
{
	if (!node)
		return ;
	if (node->left)
		free_ast_node(node->left);
	if (node->right)
		free_ast_node(node->right);
	free_command(node->cmd);
	free(node->op);
	free(node);
}

void	free_minishell(t_minishell *minishell)
{
	if (!minishell)
		return ;
	free_tokens(minishell->tokens);
	free_command_list(minishell->cmd);
	free_ast_node(minishell->ast_tree);
	if (minishell->executor)
		free(minishell->executor);
	if (minishell->env)
		free_myenv(minishell->env);
	free(minishell);
}
