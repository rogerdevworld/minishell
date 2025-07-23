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

char	*join_dir_with_slash(char *base, char *dir, int add_slash)
{
	char	*tmp;
	char	*res;

	tmp = ft_strjoin(base, dir);
	if (!tmp)
		return (NULL);
	free(base);
	if (add_slash)
	{
		res = ft_strjoin(tmp, "/");
		free(tmp);
		if (!res)
			return (NULL);
		return (res);
	}
	return (tmp);
}

// Construye path para <= 2 dirs con <=4 vars y <25 líneas
char	*build_short_path(char **dirs)
{
	char	*final;
	int		j;

	final = ft_strdup(" /");
	if (!final)
		return (NULL);
	j = 0;
	while (dirs[j])
	{
		final = join_dir_with_slash(final, dirs[j], dirs[j + 1] != 0);
		if (!final)
			return (NULL);
		j++;
	}
	return (final);
}

// Construye path con ~/... para > 2 dirs con <4 vars y <25 líneas
char	*build_long_path(char **dirs, int len)
{
	char	*tmp;
	char	*last;
	char	*final;

	tmp = ft_strjoin("/", dirs[len - 2]);
	if (!tmp)
		return (NULL);
	last = ft_strjoin(tmp, "/");
	free(tmp);
	if (!last)
		return (NULL);
	tmp = ft_strjoin(last, dirs[len - 1]);
	free(last);
	if (!tmp)
		return (NULL);
	final = ft_strjoin("~/...", tmp);
	free(tmp);
	return (final);
}

// Genera parte del prompt coloreado, variables ≤4 y líneas <25
char	*build_colored_prompt_part(char *user, char *path, int status)
{
	char	*arrow_color;
	char	*colored_arrow;
	char	*lightning;
	char	*tmp;

	arrow_color = (status == 0) ? GREEN : RED;
	colored_arrow = ft_strjoin(arrow_color, " ⟿  ");
	lightning = ft_strjoin(YELLOW, " ⚡ ");
	tmp = ft_strjoin(MAGENTA, "minishell@");
	if (!colored_arrow || !lightning || !tmp)
		return (NULL);
	tmp = ft_strjoin(tmp, CYAN);
	tmp = ft_strjoin(tmp, user);
	tmp = ft_strjoin(tmp, lightning);
	free(lightning);
	tmp = ft_strjoin(tmp, YELLOW);
	tmp = ft_strjoin(tmp, path);
	tmp = ft_strjoin(tmp, colored_arrow);
	free(colored_arrow);
	tmp = ft_strjoin(tmp, RESET);
	return (tmp);
}

char	*ft_desing(char **env, int status)
{
	char	*user;
	char	*path;
	char	*final;

	user = get_user(env);
	path = path_terminal();
	if (!user || !path)
	{
		free(user);
		free(path);
		return (NULL);
	}
	final = build_colored_prompt_part(user, path, status);
	free(user);
	free(path);
	return (final);
}
