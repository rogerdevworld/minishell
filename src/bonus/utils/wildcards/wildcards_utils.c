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

/**
 * @brief Recursively checks if a string matches a given wildcard pattern.
 * This function implements a basic wildcard matching algorithm
	where '*' matches
 * zero or more characters. It works by comparing the pattern
	and string
 * character by character, handling the '*' wildcard specifically.
 *
 * @param pattern The wildcard pattern string (e.g., "foo*.txt").
 * @param str The string to match against the pattern (e.g., "foobar.txt").
 * @return Returns 1 if the string matches the pattern, 0 otherwise.
 */
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

/**
 * @brief Extracts the path component from a given pattern string.
 *
 * This function finds the last '/' in the pattern string and returns
 * the substring before it. If no '/' is found, it means the pattern
 * is in the current directory, so it returns a duplicate of ".".
 * param pattern The full pattern string
 * return A newly allocated string containing the path (e.g., "path/to"),
 * or a duplicate of "." if no path is present,
	or NULL on memory allocation failure.
 */
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

/**
 * @brief Extracts the filename pattern component from
	a given full pattern string.
 * This function finds the last '/' in the full pattern string and returns
 * a pointer to the substring immediately after it. If no '/' is found,
 * the entire string is considered the pattern.
 *
 * @param pattern The full pattern string (e.g., "path/to/file*.txt").
 * @return A const pointer to the start of the pattern component (e.g.,
	"file*.txt").
 */
const char	*extract_pattern(const char *pattern)
{
	char	*slash;

	slash = ft_strrchr(pattern, '/');
	if (!slash)
		return (pattern);
	return (slash + 1);
}

/**
 * @brief Adds a new string value to a dynamically allocated array of strings.
 *
 * This function reallocates the given array of strings to accommodate a new
 * value. It appends the new value to the end of the array and ensures the
 * array remains null-terminated.
 *
 * @param arr A pointer to the dynamically allocated array of strings.
 * @param count A pointer to an integer representing the current number of
 * strings in `arr`. This value will be incremented.
 * @param value The new string to add to the array. It is expected to be
 * dynamically allocated and will be freed if realloc fails.
 * @return The reallocated and updated array of strings, or the original
 * array if realloc fails (in which case `value` is freed).
 */
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
