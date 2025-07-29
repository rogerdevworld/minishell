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

void	free_redir(t_redir *redir)
{
	if (!redir)
		return ;

	// --- INICIO DE LA CORRECCIÓN ---
	// Liberamos la memoria de los arrays y su contenido.
	if (redir->heredoc_fds)
		free(redir->heredoc_fds);
	if (redir->limiter)
		free_split(redir->limiter);
	if (redir->in_file)
		free_split(redir->in_file);
	if (redir->out_file)
		free_split(redir->out_file);
	free(redir);
	// --- FIN DE LA CORRECCIÓN ---
}

// void free_tokens(t_token *tokens)
// {
// 	t_token *tmp;

// 	while (tokens)
// 	{
// 		tmp = tokens->next;
// 		free(tokens->value); // libera la cadena duplicada con strdup
// 		free(tokens);        // libera el nodo
// 		tokens = tmp;
// 	}
	
// }

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
		//tmp->value = NULL;
	}
	tmp = NULL;
	// printf("Tokens Liberados\n");
}

void	free_command(t_command *cmd)
{
	if (!cmd)
		return ;
	free_split(cmd->args);
	free(cmd->path);
	if (cmd->redir)
		free_redir(cmd->redir);
	free(cmd);
	// printf("Commands Liberados\n");
}

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
	//node->cmd = NULL;
	//node->left = NULL;
	//node->right = NULL;
	node = NULL;
	// printf("Nodos Ast Liberados\n");
}

void	free_myenv(t_myenv *myenv)
{
	if (!myenv)
		return ;
	free_env_list(myenv->list_env);
	free_env_array(myenv->env);
	free(myenv);
	// printf("MyEnv Liberado\n");
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
	// printf("List de envs Liberados\n");
}
