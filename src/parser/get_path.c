/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xviladri <xviladri@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:43:57 by xviladri          #+#    #+#             */
/*   Updated: 2025/04/22 20:28:23 by xviladri         ###   ########.fr       */
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

	i = -1;
	allpath = ft_split(ft_getenv("PATH", env), ':');
	s_cmd = ft_split(cmd, ' ');
	while (allpath[++i])
	{
		path_part = ft_strjoin(allpath[i], "/");
		exec = ft_strjoin(path_part, s_cmd[0]);
		free(path_part);
		if (access(exec, F_OK | X_OK) == 0)
		{
			free_split(allpath);
			free_split(s_cmd);
			return (exec);
		}
		free(exec);
	}
	free_split(allpath);
	free_split(s_cmd);
	return (ft_strdup(cmd));
}

/*char	*remove_quotes(const char *str)
{
	int		i = 0;
	int		j = 0;
	char	quote = 0;
	char	*res;

	if (!str)
		return (NULL);

	res = malloc(ft_strlen(str) + 1);
	if (!res)
		return (NULL);

	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"') && !quote)
			quote = str[i];
		else if (str[i] == quote)
			quote = 0;
		else
			res[j++] = str[i];
		i++;
	}
	res[j] = '\0';
	return (res);
}*/
char	*remove_quotes(char *str)
{
	size_t	len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if ((str[0] == '"' && str[len - 1] == '"')
		|| (str[0] == '\'' && str[len - 1] == '\''))
		return (ft_substr(str, 1, len - 2));
	return (ft_strdup(str));
}
