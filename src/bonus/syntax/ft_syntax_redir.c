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

/**
 * Handles input redirections for a command,
	specifically `TOKEN_REDIR_IN` (input redirection '<')
 * and `TOKEN_HEREDOC` (heredoc '<<'). It adds the filename or limiter
 	to the appropriate
 * array within the command's redirection structure and updates 
 	relevant counters.
 * @param cmd A pointer to the `t_command` structure where redirection 
	information will be stored.
 * @param filename The filename or delimiter string associated with 
 	the redirection.
 * @param type The type of the redirection token (e.g., TOKEN_REDIR_IN,
	TOKEN_HEREDOC).
 * @param i An integer representing an order or count for redirection tracking.
 */
static void	handle_input_redirection(t_command *cmd, char *filename, int type,
		int i)
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

/**
 * Handles output redirections for a command,
	specifically `TOKEN_REDIR_OUT` (output redirection '>')
 * and `TOKEN_APPEND` (append redirection '>>'). It adds the filename to 
	the `out_file` array
 * within the command's redirection structure and updates relevant counters.
* @param cmd A pointer to the `t_command` structure where redirection information
	will be stored.
 * @param filename The filename string associated with the redirection.
 * @param type The type of the redirection token (e.g., TOKEN_REDIR_OUT,
	TOKEN_APPEND).
 * @param i An integer representing an order or count for redirection tracking.
 */
static void	handle_output_redirection(t_command *cmd, char *filename, int type,
		int i)
{
	if (type == TOKEN_REDIR_OUT || type == TOKEN_APPEND)
	{
		cmd->redir->out_file = add_to_array(cmd->redir->out_file, filename);
		cmd->redir->output_ord += i;
	}
}

/**
 * Parses and processes redirection tokens for a given command.
 * It iterates through the token stream, identifies redirection operators ('<',
	'>', '>>', '<<'),
 * and extracts the corresponding filenames. It then dispatches 
	to helper functions
 * `handle_input_redirection` and `handle_output_redirection` 
	to store the redirection
 * information within the `t_command` structure.
 * @param cmd A pointer to the `t_command` structure to which 
	redirections will be added.
 * @param tokens A pointer to a pointer to the current `t_token` 
	in the token stream.
 * This pointer will be advanced as tokens are consumed.
 */
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
