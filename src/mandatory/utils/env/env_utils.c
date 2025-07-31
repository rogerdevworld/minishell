/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:12:32 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/15 14:12:35 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../../include/minishell.h"

/**
 * Sorts a linked list of environment variables alphabetically by their keys.
 * This implementation uses a bubble-sort-like approach, iterating and swapping
 * adjacent nodes if they are out of order,
	resetting the traversal after each swap.
 */
void	ft_sort_export(t_env *env)
{
	t_env	*ini;
	char	*tmp;

	ini = env;
	if (!env)
		return ;
	while (env)
	{
		if (ft_strcmp(env->key, env->next->key) < 0)
		{
			tmp = env->key;
			env->key = env->next->key;
			env->next->key = tmp;
			env = ini;
		}
		else
			env = env->next;
	}
	env = ini;
}

/**
 * Creates a deep copy of an array of strings (typically the `envp` array).
 * Allocates new memory for the array and for each individual string.
 * Returns a pointer to the newly created copy,
	or NULL if memory allocation fails.
 */
char	**ft_dup_env(char **envp)
{
	int		i;
	char	**copy;

	i = 0;
	while (envp[i])
		i++;
	copy = malloc(sizeof(char *) * (i + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		copy[i] = ft_strdup(envp[i]);
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

/**
 * Adds or updates an environment variable in the shell's environment list.
 * It constructs a "key=value" string and then uses `export_add_or_update`
 * to handle the actual addition or update in the linked list.
 */
void	ft_add_env(t_myenv *myenv, const char *key, const char *value)
{
	char	*arg;
	int		len_key;
	int		len_val;

	len_key = ft_strlen(key);
	len_val = ft_strlen(value);
	arg = malloc(len_key + len_val + 2);
	if (!arg)
		return ;
	strcpy(arg, key);
	arg[len_key] = '=';
	strcpy(arg + len_key + 1, value);
	arg[len_key + len_val + 1] = '\0';
	export_add_or_update(&myenv->list_env, arg);
	free(arg);
}

/**
 * Checks if a given key exists in the linked list of 
 	environment variables.
 * Returns 1 if the key is found, 0 otherwise.
 */
int	env_has_key(t_env *env_list, const char *key)
{
	while (env_list)
	{
		if (ft_strcmp(env_list->key, key) == 0)
			return (1);
		env_list = env_list->next;
	}
	return (0);
}
