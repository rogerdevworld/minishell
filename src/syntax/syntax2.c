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
#include "../../include/minishell.h"

/*
 * comillas en la linea
 *
 */
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
/*
files
funciona con / pero si pasa mas cosas falla de momento ./ failt
*/

void	resolve_command_path(t_command *cmd, char **env)
{
	struct stat	statbuf;

	if (!cmd || !cmd->args || !cmd->args[0])
		return ;
	if (strchr(cmd->args[0], '/'))
	{
		if (access(cmd->args[0], F_OK) != 0)
		{
			perror(cmd->args[0]);
			exit(127);
		}
		if (stat(cmd->args[0], &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
		{
			ft_putstr_fd(cmd->args[0], 2);
			ft_putstr_fd(": Is a directory\n", 2);
			exit(126);
		}
		if (access(cmd->args[0], X_OK) != 0)
		{
			perror(cmd->args[0]);
			exit(126);
		}
		cmd->path = ft_strdup(cmd->args[0]);
	}
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
