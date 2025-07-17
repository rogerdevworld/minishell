/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_syntax_redir.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 20:58:33 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/16 20:58:37 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

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
