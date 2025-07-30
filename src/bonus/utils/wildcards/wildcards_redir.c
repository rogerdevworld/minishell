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

/**
 * @brief Determines the result of a wildcard expansion for redirections.
 *
 * This function handles the outcome of a wildcard expansion specifically
 * for file redirections. It checks for cases of ambiguous redirects (multiple
 * matches) or no matches, printing appropriate error messages and returning
 * NULL if an error occurs. If exactly one match is found,
	it returns that match.
 *
 * @param count The number of matches found.
 * @param matches An array of strings containing the matched file names.
 * @param pattern The original wildcard pattern.
 * @return The single matched filename if successful, or NULL on error.
 */
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

/**
 * @brief Expands a wildcard pattern specifically for redirection purposes.
 *
 * This function takes a filename pattern, which may contain wildcards,
 * and attempts to expand it to a single filename. Unlike regular wildcard
 * expansion for arguments, redirection requires exactly one match. It uses
 * helper functions to extract the path and pattern, open the directory,
 * find matches, and then processes the results using `get_wildcard_result`.
 *
 * @param pattern The wildcard pattern string for the redirection.
 * @return A newly allocated string with the expanded filename if exactly one
 * match is found, or NULL if there are zero or multiple matches, or on error.
 */
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
