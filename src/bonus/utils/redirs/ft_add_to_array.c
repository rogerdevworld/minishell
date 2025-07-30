/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_add_to_array.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 02:39:55 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/21 02:40:00 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../../include/minishell.h"

/**
 * Creates a new string array by adding a `clean_value` to an existing array.
 * It allocates a new array, copies existing elements, appends the new value,
 * and NULL-terminates the array. The old array is freed if it exists.
 * Returns the new, expanded array, or NULL if memory allocation fails.
 */
static char	**create_new_array(char **array, char *clean_value, int len)
{
	char	**new_array;
	int		i;

	i = 0;
	new_array = malloc(sizeof(char *) * (len + 2));
	if (!new_array)
	{
		free(clean_value);
		return (NULL);
	}
	while (i < len)
	{
		new_array[i] = array[i];
		i++;
	}
	new_array[len] = clean_value;
	new_array[len + 1] = NULL;
	if (array)
		free(array);
	return (new_array);
}

/**
 * Adds a string `value` to a dynamically allocated array of strings.
 * It first removes all quotes from `value` before adding it.
 * It handles the resizing and copying of the array.
 * Returns the updated array with the new value, or NULL if an error occurs.
 */
char	**add_to_array(char **array, char *value)
{
	int		len;
	char	*clean_value;

	len = 0;
	if (!value)
		return (array);
	clean_value = remove_all_quotes(value);
	if (!clean_value)
		return (array);
	while (array && array[len])
		len++;
	return (create_new_array(array, clean_value, len));
}

/**
 * Adds a string `value` (specifically a heredoc limiter) to a 
	dynamically allocated array of strings.
 * Unlike `add_to_array`, this function does not remove quotes from the `value`
 * before adding it, preserving the original limiter string.
 * It handles the resizing and copying of the array.
 * Returns the updated array with the new value, or NULL if an error occurs.
 */
char	**add_to_array_heredoc(char **array, char *value)
{
	int		len;
	char	**new_array;
	int		i;

	if (!value)
		return (array);
	len = 0;
	while (array && array[len])
		len++;
	new_array = malloc(sizeof(char *) * (len + 2));
	if (!new_array)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new_array[i] = array[i];
		i++;
	}
	new_array[len] = ft_strdup(value);
	new_array[len + 1] = NULL;
	if (array)
		free(array);
	return (new_array);
}
