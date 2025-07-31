/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_redir.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:56:13 by rmarrero          #+#    #+#             */
/*   Updated: 2025/04/09 22:22:59 by xviladri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../../include/minishell.h"

// Line count: 12
static char	*get_wildcard_result(int count, char **matches, char *pattern)
{
	if (count > 1)
	{
		msg("ambiguous redirect", pattern);
		free_split(matches);
		return (NULL);
	}
	if (count == 0)
	{
		msg("No such file or directory", pattern);
		free_split(matches);
		return (NULL);
	}
	return (matches[0]);
}

// Line count: 21
char	*expand_redir_wildcard(char *pattern)
{
	t_wildcard_data	data;
	char			**matches;
	int				count;

	if (!pattern || !ft_strchr(pattern, '*'))
		return (ft_strdup(pattern));
	matches = NULL;
	count = 0;
	data.match_found = 0;
	data.path = extract_path(pattern);
	data.pattern = extract_pattern(pattern);
	data.dir = opendir(data.path);
	if (data.dir != NULL)
	{
		find_matches_in_dir(&data, &matches, &count);
		closedir(data.dir);
	}
	free(data.path);
	return (get_wildcard_result(count, matches, pattern));
}
