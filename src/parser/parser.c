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
	cmd->operator= NULL;
	cmd->next = NULL;
	return (cmd);
}

void	handle_redirect(t_command *cmd, t_token **tokens)
{
	int	type;

	if (!tokens || !*tokens || !(*tokens)->next)
		return ;
	type = (*tokens)->type;
	*tokens = (*tokens)->next;
	if (!*tokens)
		return ;
	if (type == TOKEN_REDIR_OUT)
		cmd->output_file = ft_open((*tokens)->value, 1);
	else if (type == TOKEN_APPEND)
		cmd->output_file = ft_open((*tokens)->value, 2);
	else if (type == TOKEN_REDIR_IN)
		cmd->input_file = ft_open((*tokens)->value, 0);
	else if (type == TOKEN_HEREDOC)
	{
		cmd->limiter = ft_strdup((*tokens)->value);
		cmd->input_file = ft_open(cmd->limiter, 2);
	}
}

char	*operator_to_str(int token_type)
{
	if (token_type == TOKEN_PIPE)
		return "|";
	else if (token_type == TOKEN_AND)
		return "&&";
	else if (token_type == TOKEN_OR)
		return "||";
	else if (token_type == TOKEN_REDIR_IN)
		return "<";
	else if (token_type == TOKEN_REDIR_OUT)
		return ">";
	else if (token_type == TOKEN_APPEND)
		return ">>";
	else if (token_type == TOKEN_HEREDOC)
		return "<<";
	else if (token_type == TOKEN_OPEN_PAREN)
		return "(";
	else if (token_type == TOKEN_CLOSE_PAREN)
		return ")";
	else if (token_type == TOKEN_WORD)
		return "WORD";
	else if (token_type == TOKEN_INVALID)
		return "INVALID";
	else
		return "UNKNOWN";
}


t_command	*parse_tokens(t_token *tokens, char **envp)
{
	t_command	*cmds;
	t_command	*tmp;
	int			i;
	char		*clean_arg;

	cmds = init_command();
	tmp = cmds;
	i = 0;
	if (!tokens || !cmds)
		return (NULL);
	while (tokens)
	{
		if (tokens->type == TOKEN_WORD)
		{
			tmp->args[i++] = ft_strdup(tokens->value);
			tmp->path = get_path(tmp->args[0], envp);
			tmp->operator = ft_strdup(operator_to_str(tokens->type));

		}
		else if (tokens->type == TOKEN_REDIR_IN || tokens->type == TOKEN_REDIR_OUT || tokens->type == TOKEN_APPEND || tokens->type == TOKEN_HEREDOC)
		{
			handle_redirect(tmp, &tokens);
			if (tokens)
				tokens = tokens->next;
			continue ;
		}
		else if (tokens->type == TOKEN_AND || tokens->type == TOKEN_OR)
		{
			tmp->next = init_command();
			if (!tmp->next)
				return (NULL);
			tmp = tmp->next;
			tmp->operator = ft_strdup(operator_to_str(tokens->type));
			tmp->next = init_command();
			if (!tmp->next)
				return (NULL);
			tmp = tmp->next;
			i = 0;
		}
		tokens = tokens->next;
	}
	return (cmds);
}

void	print_command_list(t_command *cmds)
{
	int	i;
	int	k;

	k = 1;
	while (cmds)
	{
		ft_printf("NODO: %p\n", cmds);
		ft_printf("OPERADOR: %s\n", cmds->operator);
		ft_printf("--comando %i:\n", k);
		i = 0;
		while (cmds->args[i])
		{
			ft_printf("  args[%d]: %s\n", i, cmds->args[i]);
			i++;
		}
		if (cmds->path)
			ft_printf("  Path: %s\n", cmds->path);
		if (cmds->limiter)
			ft_printf("  Heredoc Limiter: %s\n", cmds->limiter);
		ft_printf("  Infile FD: %d\n", cmds->input_file);
		ft_printf("  Outfile FD: %d\n", cmds->output_file);
		cmds = cmds->next;
		k++;
	}
}

void	ft_syntax_check(t_minishell *minishell)
{
	if (!minishell)
		return ;
	if (minishell->cmd && minishell->cmd->args[0]
		&& ft_strcmp(minishell->cmd->args[0], "./minishell") == 0)
		ft_shlvl(minishell);
}