/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:01:14 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/15 14:01:17 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

char	*get_path(char *cmd, char **env)
{
	int		i;
	char	*exec;
	char	**allpath;
	char	*path_part;
	char	**s_cmd;
	char	*path_env;

	if (!cmd || cmd[0] == '\0')
        return NULL;
	i = -1;
	path_env = ft_getenv("PATH", env);
	if (!path_env)
		return (NULL);
	allpath = ft_split(path_env, ':');
	s_cmd = ft_split(cmd, ' ');
	while (allpath[++i])
	{
		path_part = ft_strjoin(allpath[i], "/");
		exec = ft_strjoin(path_part, s_cmd[0]);
		free(path_part);
		if (access(exec, F_OK | X_OK) == 0)
			return (free_split(allpath), free_split(s_cmd), exec);
		free(exec);
	}
	return (free_split(allpath), free_split(s_cmd), NULL);
}
