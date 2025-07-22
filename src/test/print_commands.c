/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 05:20:50 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/22 05:20:54 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

static void	print_args(char **args)
{
	int	i;

	if (!args || !args[0])
	{
		printf("           args: (null)\n");
		return ;
	}
	printf("           args:");
	i = 0;
	while (args[i] != NULL)
	{
		printf(" \"%s\"", args[i]);
		i++;
	}
	printf("\n");
}

static void	print_files(char **files, const char *label)
{
	int	i;

	if (!files)
	{
		printf("           %s: (null)\n", label);
		return ;
	}
	printf("           %s:", label);
	i = 0;
	while (files[i] != NULL)
	{
		printf(" \"%s\"", files[i]);
		i++;
	}
	printf("\n");
}

void	print_command(t_command *cmd)
{
	if (!cmd)
	{
		printf("Command is NULL\n");
		return ;
	}
	printf("         Command:\n");
	print_args(cmd->args);
	if (cmd->path)
		printf("           path: %s\n", cmd->path);
	else
		printf("           path:  (null)\n");
	printf("           input_file fd: %d\n", cmd->redir->input_file);
	printf("           input_file ord: %d\n", cmd->redir->input_ord);
	printf("           output_file fd: %d\n", cmd->redir->output_file);
	printf("           output_file ord: %d\n", cmd->redir->output_ord);
	printf("         Infile:\n");
	print_files(cmd->redir->in_file, "Infile");
	printf("         Outfile:\n");
	print_files(cmd->redir->out_file, "Outfile");
	printf("         Limiter:\n");
	print_files(cmd->redir->limiter, "Limiter");
}
