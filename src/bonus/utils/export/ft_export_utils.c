/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaacosta <jaacosta@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:54:37 by jaacosta          #+#    #+#             */
/*   Updated: 2025/07/22 00:54:39 by jaacosta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../../include/minishell.h"

/**
 * Prints the current environment variables in a format 
	suitable for the 'export' command.
 * It converts the linked list of environment variables to a temporary array,
 * sorts them alphabetically by key, and then prints each variable.
 * Variables with values are printed as "declare -x KEY="VALUE"",
 * while those without values are printed as "declare -x KEY".
 */
void	print_export(t_env *env)
{
	t_env	**array;
	int		i;

	array = env_to_array(env);
	if (!array)
		return ;
	sort_env_array(array);
	i = 0;
	while (array[i])
	{
		if (array[i]->content)
			ft_printf("declare -x %s=\"%s\"\n", array[i]->key,
				array[i]->content);
		else
			ft_printf("declare -x %s\n", array[i]->key);
		i++;
	}
	free(array);
}

/**
 * Joins two strings and frees the first string.
 * This is a utility function to concatenate a key and a value 
	for environment variables,
 * and then free the memory of the key string.
 * Returns a newly allocated string containing the joined content.
 */
char	*ft_strjoin_free_env(char *s1, char *s2)
{
	char	*joined;

	joined = NULL;
	joined = ft_strjoin(s1, s2);
	free(s1);
	return (joined);
}

/**
 * Calculates the number of elements in a linked list of environment 
 	variables.
 * Returns the total count of nodes in the list.
 */
int	env_list_size(t_env *env)
{
	int	size;

	size = 0;
	while (env)
	{
		size++;
		env = env->next;
	}
	return (size);
}

/**
 * Converts a linked list of environment variables into a dynamically
	allocated array of pointers to `t_env` nodes.
 * The array is null-terminated.
 * Returns the newly created array, or NULL if memory allocation fails.
 */
t_env	**env_to_array(t_env *env)
{
	int		size;
	t_env	**array;
	int		i;

	size = env_list_size(env);
	array = malloc(sizeof(t_env *) * (size + 1));
	if (!array)
		return (NULL);
	i = 0;
	while (env)
	{
		array[i] = env;
		env = env->next;
		i++;
	}
	array[i] = NULL;
	return (array);
}

/**
 * Sorts an array of pointers to `t_env` structures alphabetically
	by their `key` field.
 * This uses a simple bubble sort algorithm.
 */
void	sort_env_array(t_env **array)
{
	int		i;
	int		j;
	t_env	*tmp;

	i = 0;
	j = 0;
	while (array[i])
	{
		j = i + 1;
		while (array[j])
		{
			if (ft_strcmp(array[i]->key, array[j]->key) > 0)
			{
				tmp = array[i];
				array[i] = array[j];
				array[j] = tmp;
			}
			j++;
		}
		i++;
	}
}
