/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xviladri <xviladri@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:43:57 by xviladri          #+#    #+#             */
/*   Updated: 2025/06/29 14:42:07 by xviladri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

t_command	*init_command(void)
{
	t_command	*cmd;

	cmd = (t_command *)malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = ft_calloc(10, sizeof(char *));
	cmd->path = NULL;
	cmd->input_file = -1;
	cmd->output_file = -1;
	cmd->limiter = NULL;
	cmd->operator = NONE;
	cmd->next = NULL;
	return (cmd);
}

void	handle_redirect(t_command *cmd, t_token **tokens)
{
	if (!tokens || !*tokens || !(*tokens)->next)
		return ;
	if (ft_strncmp((*tokens)->value, ">>", 2) == 0)
	{
		*tokens = (*tokens)->next;
		cmd->output_file = ft_open((*tokens)->value, 2);
	}
	else if (ft_strncmp((*tokens)->value, "<<", 2) == 0)
	{
		*tokens = (*tokens)->next;
		cmd->limiter = ft_strdup((*tokens)->value);
		cmd->input_file = ft_open(cmd->limiter, 2);
	}
	else if (ft_strncmp((*tokens)->value, ">", 1) == 0)
	{
		*tokens = (*tokens)->next;
		cmd->output_file = ft_open((*tokens)->value, 1);
	}
	else if (ft_strncmp((*tokens)->value, "<", 1) == 0)
	{
		*tokens = (*tokens)->next;
		cmd->input_file = ft_open((*tokens)->value, 0);
	}
}

static int	check_syntax(t_token *tokens, t_token *prev)
{
	return (!(tokens->type == TOKEN_OPERATOR && prev
			&& prev->type == TOKEN_OPERATOR)
		&& !(tokens->type == TOKEN_OPERATOR && !tokens->next));
}

void	process_token(t_token **tokens, t_command *current, int *i, char **envp)
{
	char	*clean_arg;

	if (((*tokens)->type == TOKEN_COMMAND)
		|| ((*tokens)->type == TOKEN_ARGUMENT))
	{
		clean_arg = remove_quotes((*tokens)->value);
		current->args[*i] = clean_arg;
		current->path = get_path(current->args[0], envp);
		(*i)++;
	}
	else if ((*tokens)->type == TOKEN_REDIRECTION)
		handle_redirect(current, tokens);
	else if ((*tokens)->type == TOKEN_OPERATOR)
	{
		current->operator = resolve_operator((*tokens)->value);
		current->next = init_command();
		current = current->next;
		*i = 0;
	}
}

t_command	*parse_tokens(t_token *tokens, char **envp)
{
	t_command	*cmds;
	t_command	*current;
	int			i;
	t_token		*prev;

	(void)envp;
	if (!tokens || tokens-> type == TOKEN_OPERATOR || !cmds)
		return (ft_exit("syntax error near unexpected token"), NULL);
	cmds = init_command();
	current = cmds;
	i = 0;
	prev = NULL;
	while (tokens)
	{
		if (!check_syntax(tokens, prev))
			return (ft_exit("syntax error near unexpected token"),
				NULL);
		process_token(&tokens, current, &i, envp);
		prev = tokens;
		tokens = tokens->next;
	}
	return (cmds);
}
