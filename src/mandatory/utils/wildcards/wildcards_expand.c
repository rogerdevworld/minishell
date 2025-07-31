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

// Line count: 15
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

// Line count: 14
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

// Line count: 16
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
