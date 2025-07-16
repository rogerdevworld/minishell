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
#include "../../../include/minishell.h"

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		free(tmp->value);
		free(tmp);
	}
}

void	free_command(t_command *cmd)
{
	if (!cmd)
		return ;
	free_split(cmd->args);
	free(cmd->path);
	if (cmd->redir)
	{
		free(cmd->redir->limiter);
		free(cmd->redir->in_file);
		free(cmd->redir->out_file);
		free(cmd->redir);
	}
	free(cmd);
}

void	free_ast(t_ast *node)
{
	if (!node)
		return ;
	free_command(node->cmd);
	free_ast(node->left);
	free_ast(node->right);
	free(node);
}

void	free_myenv(t_myenv *myenv)
{
	if (!myenv)
		return ;
	free_env_list(myenv->list_env);
	free_env_array(myenv->env);
	free(myenv);
}

void	free_env_list(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->key);
		free(env->content);
		free(env);
		env = tmp;
	}
}
