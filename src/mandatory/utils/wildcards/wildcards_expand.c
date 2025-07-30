/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_expand.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:56:13 by rmarrero          #+#    #+#             */
/*   Updated: 2025/04/09 22:22:59 by xviladri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/minishell.h"

/**
 * @brief Adds a matched directory entry to the list of arguments.
 *
 * This function constructs the full path of a matched directory entry.
 * If the path is the current directory ".", it just adds the entry's name.
 * Otherwise, it appends the entry's name to the provided path.
 *
 * @param data A pointer to the `t_wildcard_data` structure, containing
 * information about the wildcard expansion,
	including the current path and entry.
 * @param args A pointer to a pointer to the array of arguments. The matched
 * entry's full path will be added to this array.
 * @param count A pointer to an integer representing the current number of
 * arguments in `args`. This count will be incremented.
 */
static void	add_matched_entry(t_wildcard_data *data, char ***args, int *count)
{
	char	*full_match;
	char	*tmp_path;

	data->match_found = 1;
	if (ft_strcmp(data->path, ".") == 0)
		full_match = ft_strdup(data->entry->d_name);
	else
	{
		tmp_path = ft_strjoin(data->path, "/");
		full_match = ft_strjoin(tmp_path, data->entry->d_name);
		free(tmp_path);
	}
	*args = add_arg(*args, count, full_match);
}

/**
 * @brief Finds and adds directory entries that match a given wildcard pattern.
 *
 * This function iterates through the entries in an opened directory.
 * For each entry, it checks if it starts with a dot ('.') and, if so,
 * skips it unless the pattern itself starts with a dot. Then, it attempts
 * to match the entry's name against the provided wildcard pattern.
 * Matched entries are added to the list of arguments.
 *
 * @param data A pointer to the `t_wildcard_data` structure, containing
 * the directory pointer, pattern, and entry information.
 * @param args A pointer to a pointer to the array of arguments where matches
 * will be added.
 * @param count A pointer to an integer representing the current number of
 * arguments. This count will be updated.
 */
void	find_matches_in_dir(t_wildcard_data *data, char ***args, int *count)
{
	data->entry = readdir(data->dir);
	while (data->entry)
	{
		if (data->entry->d_name[0] == '.' && data->pattern[0] != '.')
		{
			data->entry = readdir(data->dir);
			continue ;
		}
		if (match_pattern(data->pattern, data->entry->d_name))
			add_matched_entry(data, args, count);
		data->entry = readdir(data->dir);
	}
}

/**
 * @brief Expands a single argument containing a wildcard.
 *
 * This function attempts to expand a single argument that contains a wildcard
 * character. It extracts the path and the pattern from the argument, opens
 * the corresponding directory, and finds all matching entries. If no matches
 * are found, the original argument is added to the new arguments list.
 *
 * @param arg The argument string containing the wildcard to expand.
 * @param new_args A pointer to a pointer to the array of new arguments.
 * Expanded matches or the original argument will be added here.
 * @param count A pointer to an integer representing the current number of
 * arguments in `new_args`. This count will be updated.
 */
void	expand_single_wildcard(char *arg, char ***new_args, int *count)
{
	t_wildcard_data	data;

	data.match_found = 0;
	data.path = extract_path(arg);
	data.pattern = extract_pattern(arg);
	data.dir = opendir(data.path);
	if (data.dir)
	{
		find_matches_in_dir(&data, new_args, count);
		closedir(data.dir);
	}
	if (!data.match_found)
		*new_args = add_arg(*new_args, count, ft_strdup(arg));
	free(data.path);
}
