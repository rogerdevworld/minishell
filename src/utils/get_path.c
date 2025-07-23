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

// Obtiene las rutas del PATH del entorno como un array de strings
char	**get_all_paths(char **env)
{
	char	*path_env;

	path_env = ft_getenv("PATH", env);
	if (!path_env)
		return (NULL);
	return (ft_split(path_env, ':'));
}

// Busca la ruta del ejecutable para el comando usando las rutas en allpath
char	*find_executable(char *cmd, char **allpath)
{
	int		i;
	char	*exec;
	char	*path_part;
	char	**s_cmd;

	if (!cmd || cmd[0] == '\0' || !allpath)
		return (NULL);
	s_cmd = ft_split(cmd, ' ');
	if (!s_cmd)
		return (NULL);
	i = 0;
	while (allpath[i])
	{
		path_part = ft_strjoin(allpath[i], "/");
		exec = ft_strjoin(path_part, s_cmd[0]);
		free(path_part);
		if (access(exec, F_OK | X_OK) == 0)
		{
			free_split(s_cmd);
			return (exec);
		}
		free(exec);
		i++;
	}
	return (free_split(s_cmd), NULL);
}

// Función principal que combina ambas
char	*get_path(char *cmd, char **env)
{
	char	**allpath;
	char	*exec;

	allpath = get_all_paths(env);
	if (!allpath)
		return (NULL);
	exec = find_executable(cmd, allpath);
	free_split(allpath);
	return (exec);
}
