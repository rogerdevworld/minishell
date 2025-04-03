/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:05:47 by rmarrero          #+#    #+#             */
/*   Updated: 2025/04/01 13:05:50 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

char	*get_git_branch(void)
{
	FILE	*fp;
	char	branch[1024];

	fp = popen("git rev-parse --abbrev-ref HEAD 2>/dev/null", "r");
	if (!fp)
		return (NULL);
	if (!fgets(branch, sizeof(branch), fp))
	{
		pclose(fp);
		return (NULL);
	}
	pclose(fp);
	branch[strcspn(branch, "\n")] = 0;
	return (ft_strjoin("\033[31m\033[43m  ", strdup(branch)));
}

// -- design -- //
char	*path_terminal(void)
{
	char	path[1024];

	if (!getcwd(path, sizeof(path)))
		return (NULL);
	return (ft_strjoin("\033[42m", ft_strjoin(" ⚡⚙ ", ft_strjoin(ft_strdup(path), ">\033[0m"))));
}

char	*meta_path(void)
{
	return (ft_strjoin((ft_strjoin(path_terminal(), get_git_branch())), ">\033[0m "));
}