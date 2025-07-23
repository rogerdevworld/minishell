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
#include "../../../include/minishell.h"

/**
 * to clean the files
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
 * to recieve the limitter of the heredoc with quotes
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
	new_array[len] = value;
	new_array[len + 1] = NULL;
	if (array)
		free(array);
	return (new_array);
}
