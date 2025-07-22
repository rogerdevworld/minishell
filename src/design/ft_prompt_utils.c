/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_in_shell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:05:47 by rmarrero          #+#    #+#             */
/*   Updated: 2025/04/03 13:37:25 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

char	*path_terminal(void)
{
	char	*cwd;
	char	**dirs;
	char	*final;
	int		len;

	cwd = malloc(1024);
	if (!cwd)
		return (NULL);
	if (!getcwd(cwd, 1024))
	{
		free(cwd);
		return (NULL);
	}
	dirs = ft_split(cwd, '/');
	free(cwd);
	if (!dirs)
		return (NULL);
	len = 0;
	while (dirs[len])
		len++;
	if (len <= 2)
		final = build_short_path(dirs);
	else
		final = build_long_path(dirs, len);
	return (free_split(dirs), final);
}

char	*get_user(char **envp)
{
	char	*user;

	if (!envp)
		return (ft_strdup("Roger"));
	user = ft_getenv("USER", envp);
	if (!user)
		return (NULL);
	return (user);
}