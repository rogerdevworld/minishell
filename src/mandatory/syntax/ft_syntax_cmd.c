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
#include "../../../include/minishell.h"

/**
 * Parses a simple command from a stream of tokens.
 * It extracts command arguments and redirection information.
 * It also determines the full path to the command if it's executable.
 * Returns an AST node representing the parsed command.
 */
t_ast	*parse_simple_command(t_token **tokens, char **envp)
{
	t_command	*cmd;
	int			i;
	t_ast		*tmp_node;

	i = 0;
	cmd = init_command();
	while (*tokens)
	{
		if ((*tokens)->type == TOKEN_WORD && (*tokens)->value)
		{
			cmd->args[i++] = ft_strdup((*tokens)->value);
			*tokens = (*tokens)->next;
		}
		else if ((*tokens)->type == TOKEN_REDIR_IN || (*tokens)->type
			== TOKEN_REDIR_OUT || (*tokens)->type == TOKEN_APPEND
			|| (*tokens)->type == TOKEN_HEREDOC)
			ft_redirects(cmd, tokens);
		else
			break ;
	}
	cmd->args[i] = NULL;
	if (cmd->args[0] && ft_strncmp(cmd->args[0], ".", 1) != 0)
		cmd->path = get_path(cmd->args[0], envp);
	tmp_node = init_ast_node(NODE_COMMAND, cmd);
	return (tmp_node);
}
