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
#include "../../../include/minishell.h"

static void	handle_input_redirection(t_command *cmd, char *filename,
		int type, int i)
{
	if (type == TOKEN_REDIR_IN)
	{
		cmd->redir->in_file = add_to_array(cmd->redir->in_file, filename);
		cmd->redir->input_ord += i;
	}
	else if (type == TOKEN_HEREDOC)
	{
		cmd->redir->limiter = add_to_array_heredoc(cmd->redir->limiter,
				filename);
		cmd->redir->heredoc_count++;
	}
}

// Esta función ya existe, solo hay que modificarla
static void	handle_output_redirection(t_command *cmd, char *filename,
		int type, int i)
{
	int	count;

	if (type == TOKEN_REDIR_OUT || type == TOKEN_APPEND)
	{
		count = 0;
		while (cmd->redir->out_file && cmd->redir->out_file[count])
			count++;
		cmd->redir->out_file = add_to_array(cmd->redir->out_file, filename);
		cmd->redir->out_file_type[count] = type;
		cmd->redir->output_ord += i;
	}
}

void	ft_redirects(t_command *cmd, t_token **tokens)
{
	int		type;
	int		i;
	char	*filename;

	i = 1;
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
		handle_input_redirection(cmd, filename, type, i);
		handle_output_redirection(cmd, filename, type, i);
		i++;
		if (i == 3)
			i = 100;
		next_token(tokens);
	}
}
