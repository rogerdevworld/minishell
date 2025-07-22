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

static int	prepare_path_and_cmd(char *cmd, char **env, char ***allpath,
		char ***s_cmd)
{
	char	*path_env;

	if (!cmd || cmd[0] == '\0')
		return (0);
	path_env = ft_getenv("PATH", env);
	if (!path_env)
		return (0);
	*allpath = ft_split(path_env, ':');
	if (!*allpath)
		return (0);
	*s_cmd = ft_split(cmd, ' ');
	if (!*s_cmd)
	{
		free_split(*allpath);
		return (0);
	}
	return (1);
}

char	*get_path(char *cmd, char **env)
{
	char	**allpath;
	char	**s_cmd;
	char	*path_part;
	char	*exec;
	int		i;

	if (!prepare_path_and_cmd(cmd, env, &allpath, &s_cmd))
		return (NULL);
	i = -1;
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

/* char	*get_path(char *cmd, char **env)
{
	int		i;
	char	*exec;
	char	**allpath;
	char	*path_part;
	char	**s_cmd;
	char	*path_env;

	if (!cmd || cmd[0] == '\0')
		return (NULL);
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
} */
