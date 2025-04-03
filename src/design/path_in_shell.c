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
	return (ft_strjoin("\033[42m", ft_strjoin(" ⚡⚙ ", ft_strjoin(ft_strdup(path), "\033[43m\033[32m\033[0m"))));
}

char *get_user(char **envp)
{
    char *user = ft_getenv("USER", envp);
    if (!user) 
        return (NULL);
		return ft_strjoin(
			ft_strjoin("\033[44m", 
				ft_strjoin(
					ft_strjoin(ft_strdup(user), "@"), 
					get_os()
				)
			), 
			"\033[42m\033[34m\033[0m"
		);
		
}

const char *get_os(void)
{
#ifdef _WIN32
    return "Windows";
#elif __APPLE__
    return "macOS";
#elif __linux__
    return "Linux";
#elif __unix__
    return "Unix";
#else
    return "Desconocido";
#endif
}

char	*meta_path(char **envp)
{
	return (ft_strjoin(get_user(envp), ft_strjoin((ft_strjoin(path_terminal(), get_git_branch())), "\033[0m\033[33m\033[0m\n")));
}