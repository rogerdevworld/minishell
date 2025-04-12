/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 21:49:28 by rmarrero          #+#    #+#             */
/*   Updated: 2025/02/22 21:49:33 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/minishell.h"

int	match_pattern(const char *pattern, const char *str)
{
	if (!*pattern)
		return (!*str);
	if (*pattern == '*')
	{
		while (*pattern == '*')
			pattern++;
		if (!*pattern)
			return (1);
		while (*str)
		{
			if (match_pattern(pattern, str))
				return (1);
			str++;
		}
		return (0);
	}
	else if (*pattern == *str)
		return (match_pattern(pattern + 1, str + 1));
	else
		return (0);
}

char	*extract_path(const char *pattern)
{
	char	*slash;
	size_t	len;
	char	*path;

	slash = ft_strrchr(pattern, '/');
	if (!slash)
		return (ft_strdup("."));
	len = slash - pattern;
	path = malloc(len + 1);
	if (!path)
		return (NULL);
	ft_strlcpy(path, pattern, len + 1);
	return (path);
}

const char	*extract_pattern(const char *pattern)
{
	char	*slash;

	slash = ft_strrchr(pattern, '/');
	if (!slash)
		return (pattern);
	return (slash + 1);
}

void	ft_wildcards(char ***args)
{
	char **new_args = NULL;
	int arg_count = 0;
	DIR *dir;
	struct dirent *entry;
	int j = 0;

	while ((*args)[j])
	{
		if (ft_strchr((*args)[j], '*'))
		{
			char *path = extract_path((*args)[j]);
			const char *pat = extract_pattern((*args)[j]);
			// ft_printf("path: %s \npat: %s\n", path, pat);
			dir = opendir(path);
			if (!dir)
			{
				perror("opendir");
				free(path);
				exit(EXIT_FAILURE);
			}
			while ((entry = readdir(dir)))
			{
				if (match_pattern(pat, entry->d_name))
				{
					size_t full_len = ft_strlen(path) + ft_strlen(entry->d_name)
						+ 2;
					char *full = malloc(full_len);
					if (!full)
						exit(EXIT_FAILURE);
					ft_strlcpy(full, path, full_len);
					ft_strlcat(full, "/", full_len);
					ft_strlcat(full, entry->d_name, full_len);
					new_args = ft_realloc(new_args, sizeof(char *) * (arg_count
								+ 2));
					new_args[arg_count++] = full;
				}
			}
			closedir(dir);
			free(path);
		}
		else
		{
			new_args = ft_realloc(new_args, sizeof(char *) * (arg_count + 2));
			new_args[arg_count++] = ft_strdup((*args)[j]);
		}
		j++;
	}
	new_args[arg_count] = NULL;
	j = 0;
	while ((*args)[j])
		free((*args)[j++]);
	free(*args);
	*args = new_args;
}