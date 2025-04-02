/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xviladri <xviladri@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:43:57 by xviladri          #+#    #+#             */
/*   Updated: 2025/04/01 14:11:55 by xviladri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_command	*init_command(void)
{
	t_command	*cmd;

	cmd = (t_command *)malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = ft_calloc(10, sizeof(char *));
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->append = 0;
	cmd->next = NULL;
	return (cmd);
}

void	handle_redirect(t_command *cmd, t_token **tokens)
{
	if (ft_strcmp((*tokens)->value, ">") == 0)
	{
		*tokens = (*tokens)->next;
		cmd->output_file = ft_strdup((*tokens)->value);
		cmd->append = 0;
	}
	else if (ft_strcmp((*tokens)->value, ">>") == 0)
	{
		*tokens = (*tokens)->next;
		cmd->output_file = ft_strdup((*tokens)->value);
		cmd->append = 1;
	}
	else if (ft_strcmp((*tokens)->value, "<") == 0)
	{
		*tokens = (*tokens)->next;
		cmd->input_file = ft_strdup((*tokens)->value);
		cmd->append = 0;
	}
}

t_command	*parse_tokens(t_token *tokens)
{
	t_command	*cmds;
	t_command	*current;
	int			i;

	cmds = init_command();
	if (!cmds)
		return (NULL);
	current = cmds;
	i = 0;
	while (tokens)
	{
		if (tokens->type == TOKEN_COMMAND)
			current->args[i++] = ft_strdup(tokens->value);
		else if (tokens->type == TOKEN_REDIRECTION)
			handle_redirect(current, &tokens);
		else if (tokens->type == TOKEN_OPERATOR && ft_strcmp(tokens->value,
				"|") == 0)
		{
			current->next = init_command();
			current = current->next;
			i = 0;
		}
		tokens = tokens->next;
	}
	return (cmds);
}

void	print_command_list(t_command *cmds)
{
	int	i;

	while (cmds)
	{
		printf("Command: ");
		i = 0;
		while (cmds->args[i])
			printf("%s ", cmds->args[i++]);
		printf("\n");
		if (cmds->input_file)
			printf("  Input: %s\n", cmds->input_file);
		if (cmds->output_file)
			printf("  Output: %s (append: %d)\n", cmds->output_file,
				cmds->append);
		printf("----------------------------------------\n");
		cmds = cmds->next;
	}
}
