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
#include "../../include/minishell.h"

t_command	*init_command(void)
{
	t_command	*cmd;

	cmd = (t_command *)malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = ft_calloc(10, sizeof(char *));
	cmd->path = NULL;
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

t_command	*parse_tokens(t_token *tokens, char **envp)
{
	t_command	*cmds;
	t_command	*current;
	int			i;

	envp = envp;
	cmds = init_command();
	if (!cmds)
		return (NULL);
	current = cmds;
	i = 0;
	while (tokens)
	{
		if (tokens->type == TOKEN_COMMAND)
		{
			current->args[i] = ft_strdup(tokens->value);
			current->path = get_path(current->args[i], envp);
			i++;
		}
		else if (tokens->type == TOKEN_REDIRECTION)
			handle_redirect(current, &tokens);
		else if (tokens->type == TOKEN_OPERATOR)
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
	int	j;
	int	k;

	k = 1;
	while (cmds)
	{
		i = 0;
		j = 0;
		ft_printf("comandos + flag %i: ", k);
		while (cmds->args[i])
		{
			while (cmds->args[j])
			{
				ft_printf("%s -> ", cmds->args[j]);
				j++;
			}
			if (!cmds->path)
				printf("\nminishell: %s: commad not found\n", cmds->args[i]);
			else
			{
				ft_printf("\nCommand correcto: ");
				ft_printf("%i: %s\n", i, cmds->args[i]);
				ft_printf("%s: %s\n", cmds->args[i], cmds->path);
			}
			i++;
		}
		k++;
		ft_printf("\n");
		if (cmds->input_file)
			ft_printf("  Input: %s\n", cmds->input_file);
		if (cmds->output_file)
			ft_printf("  Output: %s (append: %d)\n", cmds->output_file,
				cmds->append);
		cmds = cmds->next;
	}
}
