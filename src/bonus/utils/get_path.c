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
#include "../../../include/minishell.h"

/**
 * @brief Retrieves all directories from the PATH environment variable.
 * This function searches the provided environment array 
	for the "PATH" variable.
 * If found, it splits the value of "PATH" by the colon delimiter (':') to
 * create an array of strings, where each string is a directory path.
 *
 * @param env The environment variables as a null-terminated array of strings.
 * @return A dynamically allocated null-terminated array of strings, where each
 * string is a directory from the PATH. Returns NULL if "PATH" is not found
 * or if memory allocation fails.
 */
char	**get_all_paths(char **env)
{
	char	*path_env;

	path_env = ft_getenv("PATH", env);
	if (!path_env)
		return (NULL);
	return (ft_split(path_env, ':'));
}

/**
 * @brief Searches for the executable path of a command within a
	list of directories.
 * This function takes a command name and an array of directory 
	paths. It constructs
 * potential full paths for the executable by joining the directory 
	paths with the
 * command name. It then checks if the constructed path points 
	to an executable file
 * that exists and has execute permissions.
 * @param cmd The command name (e.g., "ls", "grep").
 * @param allpath A null-terminated array of directory paths obtained from PATH.
 * @return A dynamically allocated string containing the full path to 
	the executable
 * if found and accessible. Returns NULL if the command is not found, is empty,
 * or if memory allocation fails. The caller is responsible for 
	freeing the returned string.
 */
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

/**

 * @brief Combines `get_all_paths` and `find_executable` to get 
	the full path of a command.
 * This is the main function for resolving a command's executable path.
 * It first retrieves all directories from the "PATH" environment variable,
 * then uses these directories to search for the specified command.
 *
 * @param cmd The command name to find (e.g., "ls").
 * @param env The environment variables as a null-terminated array of strings.
 * @return A dynamically allocated string containing the full 
 	path to the executable
 * if found and accessible. Returns NULL if the command cannot be found or
 * if memory allocation fails at any stage. The caller 
	is responsible for freeing
 * the returned string.
 */
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
