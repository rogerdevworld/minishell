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

char	*get_git_branch(void)
{
	FILE	*fp;
	char	*branch;

	branch = malloc(1024 * sizeof(char));
	if (!branch)
		return (NULL);
	fp = popen("git rev-parse --abbrev-ref HEAD 2>/dev/null", "r");
	if (!fp)
	{
		free(branch);
		return (NULL);
	}
	if (!fgets(branch, 1024, fp))
	{
		free(branch);
		pclose(fp);
		return (NULL);
	}
	pclose(fp);
	branch[strcspn(branch, "\n")] = 0;
	return (branch);
}

char	*path_terminal(void)
{
	char	*path;
	char	**dirs;
	char	*final;
	int		i;
	int		j;
	char	*tmp;
	char	*last;

	i = 0;
	path = malloc(1024);
	if (!path)
		return (NULL);
	if (!getcwd(path, 1024))
	{
		free(path);
		return (NULL);
	}
	dirs = ft_split(path, '/');
	free(path);
	if (!dirs)
		return (NULL);
	while (dirs[i])
		i++;
	if (i <= 2)
	{
		// reconstruir el path original con /
		j = 0;
		final = ft_strdup(" /");
		while (dirs[j])
		{
			tmp = ft_strjoin(final, dirs[j]);
			free(final);
			final = tmp;
			if (dirs[j + 1])
			{
				tmp = ft_strjoin(final, " /");
				free(final);
				final = tmp;
			}
			j++;
		}
	}
	else
	{
		// mostrar solo los dos últimos directorios
		tmp = ft_strjoin("/", dirs[i - 2]);
		last = ft_strjoin(tmp, "/");
		free(tmp);
		tmp = ft_strjoin(last, dirs[i - 1]);
		free(last);
		final = ft_strjoin(" ...", tmp);
		free(tmp);
	}
	// liberar dirs
	i = 0;
	while (dirs[i])
		free(dirs[i++]);
	free(dirs);
	return (final);
}

char	*get_user(char **envp)
{
	char	*user;

	user = ft_getenv("USER", envp);
	if (!user)
		return (NULL);
	return (user);
}

char	*get_computer_name(void)
{
	static char	hostname[1024];
	char *str = malloc(7);
	if (gethostname(hostname, sizeof(hostname)) != 0)
	{
		return ("Desconocido");
	}
	ft_strlcpy(str, hostname, 7);
	return (str);
}

char	*meta_path(char **envp)
{
	char	*prompt[11];
	char	*result;

	prompt[0] = "\033[44m ⚙ ⚡";
	prompt[1] = get_user(envp);
	prompt[2] = "@";
	prompt[3] = get_computer_name();
	prompt[4] = "\033[42m\033[34m\033[0m";
	prompt[5] = "\033[42m";
	prompt[6] = path_terminal();
	if (get_git_branch())
	{
		prompt[7] = " \033[43m\033[32m\033[0m";
		prompt[8] = "\033[31m\033[43m   ";
		prompt[9] = get_git_branch();
		prompt[10] = " ● \033[0m\033[33m\033[0m\n$";
	}
	else
	{
		prompt[7] = " \033[0m\033[32m\033[0m";
		prompt[8] = "\033[0m\n$";
		prompt[9] = NULL;
		prompt[10] = NULL;
	}
	result = ft_strjoin(prompt[0], ft_strjoin(prompt[1], ft_strjoin(prompt[2],
					ft_strjoin(prompt[3], ft_strjoin(prompt[4],
							ft_strjoin(prompt[5], ft_strjoin(prompt[6],
									ft_strjoin(prompt[7], ft_strjoin(prompt[8],
											ft_strjoin(prompt[9],
												prompt[10]))))))))));
	free(prompt[6]);
	free(prompt[9]);
	return (result);
}
