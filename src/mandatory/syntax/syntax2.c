/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:22:35 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/15 14:22:38 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../include/minishell.h"

int	check_unclosed_quotes(char *line)
{
	int		i;
	char	quote;

	i = 0;
	quote = 0;
	while (line[i])
	{
		if ((line[i] == '\'' || line[i] == '"') && quote == 0)
			quote = line[i];
		else if (line[i] == quote)
			quote = 0;
		i++;
	}
	if (quote)
	{
		ft_printf("Syntax error: unclosed quote %c\n", quote);
		return (2);
	}
	return (0);
}

static void	handle_direct_path(t_command *cmd)
{
	struct stat	statbuf;

	if (access(cmd->args[0], F_OK) != 0)
	{
		perror(cmd->args[0]);
		exit(127);
	}
	if (stat(cmd->args[0], &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
	{
		ft_putstr_fd(cmd->args[0], 2);
		ft_putstr_fd(": Is a directory\n", 2);
		exit(EXIT_NOT_EXECUTABLE);
	}
	if (access(cmd->args[0], X_OK) != 0)
	{
		perror(cmd->args[0]);
		exit(EXIT_NOT_EXECUTABLE);
	}
	cmd->path = ft_strdup(cmd->args[0]);
}

void	resolve_command_path(t_command *cmd, char **env)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return ;
	if (ft_strchr(cmd->args[0], '/'))
		handle_direct_path(cmd);
	else
	{
		cmd->path = get_path(cmd->args[0], env);
		if (!cmd->path)
		{
			ft_putstr_fd(cmd->args[0], 2);
			ft_putstr_fd(": command not found\n", 2);
			exit(127);
		}
	}
}
