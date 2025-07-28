/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <rmarrero@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:43:57 by rmarrero          #+#    #+#             */
/*   Updated: 2025/04/22 20:24:20 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

// t_ast	*parse_simple_command(t_token **tokens, char **envp)
// {
// 	t_command	*cmd;
// 	int			i;
// 	t_token		*tmp;	

// 	tmp = *tokens;
// 	cmd = init_command();
// 	i = 0;
// 	while (tmp)
// 	{
// 		if (tmp->type == TOKEN_WORD && tmp->value)
// 		{
// 			cmd->args[i++] = ft_strdup(tmp->value);
// 			tmp = tmp->next;
// 			//next_token(tokens);
// 		}
// 		else if (tmp->type == TOKEN_REDIR_IN
// 			|| tmp->type == TOKEN_REDIR_OUT
// 			|| tmp->type == TOKEN_APPEND
// 			|| tmp->type == TOKEN_HEREDOC)
// 			ft_redirects(cmd, &tmp);
// 		else
// 			break ;
// 	}
// 	cmd->args[i] = NULL;
// 	if (cmd->args[0])
// 		cmd->path = get_path(cmd->args[0], envp);
// 	*tokens = tmp;
// 	return (init_ast_node(NODE_COMMAND, cmd));
// }


t_ast	*parse_simple_command(t_token **tokens, char **envp)
{
	t_command	*cmd;
	// t_token		*tmp;
	int			i;
	t_ast	*tmp_node;

	cmd = NULL;
	// tmp = NULL;
	// tmp = *tokens;
	i = 0;
	cmd = init_command();
	while (*tokens)
	{
		if ((*tokens)->type == TOKEN_WORD && (*tokens)->value)
		{
			cmd->args[i++] = ft_strdup((*tokens)->value);
			*tokens = (*tokens)->next;
			// next_token(tokens);
		}
		else if ((*tokens)->type == TOKEN_REDIR_IN
			|| (*tokens)->type == TOKEN_REDIR_OUT
			|| (*tokens)->type == TOKEN_APPEND
			|| (*tokens)->type == TOKEN_HEREDOC)
			ft_redirects(cmd, tokens);
		else
			break ;
	}
	cmd->args[i] = NULL;
	if (cmd->args[0])
		cmd->path = get_path(cmd->args[0], envp);
	//tokens = tmp;
	tmp_node = init_ast_node(NODE_COMMAND, cmd);
	//free_command(cmd);
	// return (init_ast_node(NODE_COMMAND, cmd));
	return (tmp_node);
}
