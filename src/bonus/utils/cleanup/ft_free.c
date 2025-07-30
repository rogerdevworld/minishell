/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 13:35:52 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/15 13:35:55 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../../include/minishell.h"

/**
 * Frees all memory associated with a `t_redir` structure.
 * This includes deallocating the arrays for heredoc file descriptors,
 * limiters, input files, and output files,
	then finally freeing the `t_redir` structure itself.
 */
void	free_redir(t_redir *redir)
{
	if (!redir)
		return ;
	if (redir->heredoc_fds)
		free(redir->heredoc_fds);
	if (redir->limiter)
		free_split(redir->limiter);
	if (redir->in_file)
		free_split(redir->in_file);
	if (redir->out_file)
		free_split(redir->out_file);
	free(redir);
}

/**
 * Frees all nodes in a linked list of `t_token` structures.
 * It iterates through the list,
	freeing the `value` string and then the `t_token` node 
	itself for each element.
 */
void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	if (!tokens)
		return ;
	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		free(tmp->value);
		free(tmp);
	}
	tmp = NULL;
}

/**
 * Frees all memory associated with a `t_command` structure.
 * This includes deallocating the arguments array, the command path,
 * and recursively freeing the associated `t_redir` structure if it exists,
 * then finally freeing the `t_command` structure itself.
 */
void	free_command(t_command *cmd)
{
	if (!cmd)
		return ;
	free_split(cmd->args);
	free(cmd->path);
	if (cmd->redir)
		free_redir(cmd->redir);
	free(cmd);
}

/**
 * Recursively frees all nodes in an Abstract Syntax Tree (AST).
 * It traverses the tree in a post-order manner, first freeing the `t_command`
 * associated with a node (if any),
	then recursively freeing its left and right children,
 * and finally freeing the `t_ast` node itself.
 */
void	free_ast(t_ast *node)
{
	if (!node)
		return ;
	if (node->cmd)
		free_command(node->cmd);
	if (node->left)
		free_ast(node->left);
	if (node->right)
		free_ast(node->right);
	free(node);
	node = NULL;
}

/**
 * Frees all memory associated with the main `t_myenv` structure.
 * This includes deallocating the linked list of 
	environment variables (`list_env`)
 * and the `char** env` array,
	then finally freeing the `t_myenv` structure itself.
 */
void	free_myenv(t_myenv *myenv)
{
	if (!myenv)
		return ;
	free_env_list(myenv->list_env);
	free_env_array(myenv->env);
	free(myenv);
}
