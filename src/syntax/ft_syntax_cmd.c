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

// Comando base (WORD, args, redirs)
t_ast	*parse_simple_command(t_token **tokens, char **envp)
{
	t_command	*cmd;
	int			i;

	t_token *tmp;

	tmp = *tokens;
	cmd = init_command();
	i = 0;
	while (tmp)
	{
		if (tmp->type == TOKEN_WORD && tmp->value)
		{
			cmd->args[i++] = ft_strdup(tmp->value);
			tmp = tmp->next;
			//next_token(tokens);
		}
		else if (tmp->type == TOKEN_REDIR_IN
			|| tmp->type == TOKEN_REDIR_OUT
			|| tmp->type == TOKEN_APPEND
			|| tmp->type == TOKEN_HEREDOC)
			ft_redirects(cmd, &tmp);
		else
			break ;
	}
	cmd->args[i] = NULL;
	if (cmd->args[0])
		cmd->path = get_path(cmd->args[0], envp);
	return (init_ast_node(NODE_COMMAND, cmd));
}
