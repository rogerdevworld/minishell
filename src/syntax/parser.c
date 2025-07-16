/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xviladri <xviladri@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:43:57 by xviladri          #+#    #+#             */
/*   Updated: 2025/04/22 20:24:20 by xviladri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

// -- init -- //
t_command	*init_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = ft_calloc(64, sizeof(char *));
	cmd->path = NULL;
	cmd->redir = malloc(sizeof(t_redir));
	if (!cmd->redir)
	{
		free(cmd->args);
		free(cmd);
		return (NULL);
	}
	init_redir(cmd->redir);
	return (cmd);
}

t_ast	*init_ast_node(t_node_type type, t_command *cmd)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = type;
	node->cmd = cmd;
	node->left = NULL;
	node->right = NULL;
	return (node);
}
// Avanza al siguiente token
void	next_token(t_token **tokens)
{
	if (*tokens)
		*tokens = (*tokens)->next;
}

// Redirecciones
void	ft_redirects(t_command *cmd, t_token **tokens)
{
	int		type;
	char	*filename;

	while (*tokens && ((*tokens)->type == TOKEN_REDIR_IN
			|| (*tokens)->type == TOKEN_REDIR_OUT
			|| (*tokens)->type == TOKEN_APPEND
			|| (*tokens)->type == TOKEN_HEREDOC))
	{
		type = (*tokens)->type;
		next_token(tokens);
		if (!*tokens || !(*tokens)->value)
			return ;
		filename = (*tokens)->value;

		if (type == TOKEN_REDIR_IN)
			cmd->redir->in_file = add_to_array(cmd->redir->in_file, filename);
		else if (type == TOKEN_REDIR_OUT || type == TOKEN_APPEND)
			cmd->redir->out_file = add_to_array(cmd->redir->out_file, filename);
		else if (type == TOKEN_HEREDOC)
		{
			cmd->redir->limiter = add_to_array(cmd->redir->limiter, filename);
			cmd->redir->heredoc_count++;
		}
		next_token(tokens);
	}
}

// void ft_redirects(t_command *cmd, t_token **tokens)
// {
// 	int	type;
// 	char *filename;

// 	while (*tokens && ((*tokens)->type == TOKEN_REDIR_IN
// 			|| (*tokens)->type == TOKEN_REDIR_OUT
// 			|| (*tokens)->type == TOKEN_APPEND
// 			|| (*tokens)->type == TOKEN_HEREDOC))
// 	{
// 		type = (*tokens)->type;
// 		next_token(tokens);
// 		if (!*tokens)
// 			return ;
// 		filename = (*tokens)->value;

// 		if (type == TOKEN_REDIR_IN)
// 			cmd->redir->in_file = add_to_array(cmd->redir->in_file, filename);
// 		else if (type == TOKEN_REDIR_OUT || type == TOKEN_APPEND)
// 			cmd->redir->out_file = add_to_array(cmd->redir->out_file, filename);
// 		else if (type == TOKEN_HEREDOC)
// 		{
// 			if (cmd->redir->limiter)
// 				free(cmd->redir->limiter);
// 			cmd->redir->limiter = ft_strdup(filename);
// 		}
// 		next_token(tokens);
// 	}
// }


// Comando base (WORD, args, redirs)
t_ast	*parse_simple_command(t_token **tokens, char **envp)
{
	t_command	*cmd;
	int			i;

	cmd = init_command();
	i = 0;
	while (*tokens)
	{
		if ((*tokens)->type == TOKEN_WORD && (*tokens)->value)
		{
			// cmd->args[i++] = ft_strdup(remove_quotes((*tokens)->value));
			cmd->args[i++] = ft_strdup((*tokens)->value);
			next_token(tokens);
		}
		else if ((*tokens)->type == TOKEN_REDIR_IN
			|| (*tokens)->type == TOKEN_REDIR_OUT
			|| (*tokens)->type == TOKEN_APPEND
			|| (*tokens)->type == TOKEN_HEREDOC)
		{
			ft_redirects(cmd, tokens);
		}
		else
			break ;
	}
	cmd->args[i] = NULL;
	if (cmd->args[0])
		cmd->path = get_path(cmd->args[0], envp);
	return (init_ast_node(NODE_COMMAND, cmd));
}

// Paréntesis o comando base
t_ast	*parse_factor(t_token **tokens, char **envp)
{
	t_ast	*subtree;
	t_ast	*node;

	if (!*tokens)
		return (NULL);
	if ((*tokens)->type == TOKEN_OPEN_PAREN)
	{
		next_token(tokens);
		subtree = parse_expression(tokens, envp);
		if (*tokens && (*tokens)->type == TOKEN_CLOSE_PAREN)
			next_token(tokens);
		node = init_ast_node(NODE_SUBSHELL, NULL);
		node->left = subtree;
		return (node);
	}
	else
		return (parse_simple_command(tokens, envp));
}

// Pipes: a | b | c
t_ast	*parse_pipeline(t_token **tokens, char **envp)
{
	t_ast	*left;
	t_ast	*right;
	t_ast	*new_node;

	left = parse_factor(tokens, envp);
	while (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		next_token(tokens);
		right = parse_factor(tokens, envp);
		new_node = init_ast_node(NODE_PIPE, NULL);
		new_node->left = left;
		new_node->right = right;
		left = new_node;
	}
	return (left);
}

// && y ||
t_ast	*parse_expression(t_token **tokens, char **envp)
{
	t_ast *left;
	t_ast *new;
	t_node_type type;

	left = parse_pipeline(tokens, envp);
	while (*tokens && ((*tokens)->type == TOKEN_AND
			|| (*tokens)->type == TOKEN_OR))
	{
		if ((*tokens)->type == TOKEN_AND)
			type = NODE_AND;
		else
			type = NODE_OR;
		next_token(tokens);
		new = init_ast_node(type, NULL);
		new->left = left;
		new->right = parse_pipeline(tokens, envp);
		left = new;
	}
	return (left);
}