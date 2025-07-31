/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:56:13 by rmarrero          #+#    #+#             */
/*   Updated: 2025/04/09 22:22:59 by xviladri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../../include/minishell.h"

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

char	**add_arg(char **arr, int *count, char *value)
{
	char	**new_arr;

	new_arr = realloc(arr, sizeof(char *) * (*count + 2));
	if (!new_arr)
	{
		free(value);
		return (arr);
	}
	new_arr[*count] = value;
	(*count)++;
	new_arr[*count] = NULL;
	return (new_arr);
}
