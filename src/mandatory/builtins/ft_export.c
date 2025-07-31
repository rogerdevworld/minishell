/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:05:47 by rmarrero          #+#    #+#             */
/*   Updated: 2025/04/22 20:32:08 by xviladri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../include/minishell.h"

/**
 * Checks if a string is a valid identifier for an environment variable.
 * An identifier must start with a letter or underscore and contain only
 * alphanumeric characters or underscores. It cannot start with a digit or '='.
 * Returns 1 if valid, 0 otherwise.
 */
int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || !str[0] || ft_isdigit(str[0]) || str[0] == '=')
		return (0);
	i = 0;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

/**
 * Searches for an environment variable by its key in a linked list.
 * Returns a pointer to the t_env node if found, otherwise returns NULL.
 */
t_env	*find_env_var(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

/**

 * Adds a new environment variable or updates an existing one based 
 	on the argument.
 * Handles parsing the argument into key and value, validates the identifier,
 * and performs variable expansion on the value if necessary.
 * Returns 0 on success, or 1 on failure (e.g., invalid identifier).
 */
int	export_add_or_update(t_env **env_list, char *arg)
{
	char	*key;
	char	*value;
	int		mode;
	char	*expanded;

	if (!parse_export_arg(arg, &key, &value, &mode))
		return (1);
	if (!is_valid_identifier(key))
	{
		print_invalid_identifier_error(arg);
		return (free(key), free(value), 1);
	}
	if (value && check_expansion(value))
	{
		expanded = ft_expand_arg_ini(value, *env_list, 0);
		if (expanded)
		{
			free(value);
			value = expanded;
		}
	}
	update_or_add_env(env_list, key, value, mode);
	free(key);
	return (0);
}

// int	export_add_or_update(t_env **env_list, char *arg)
// {
// 	char	*key;
// 	char	*value;
// 	int		has_equal;
// 	char	*expanded;

// 	if (!parse_export_arg(arg, &key, &value, &has_equal))
// 		return (1);
// 	if (!is_valid_identifier(key))
// 	{
// 		print_invalid_identifier_error(arg);
// 		return (free(key), free(value), 1);
// 	}
// 	if (value && check_expansion(value))
// 	{
// 		expanded = ft_expand_arg_ini(value, *env_list, 0);
// 		if (expanded)
// 		{
// 			free(value);
// 			value = expanded;
// 		}
// 	}
// 	update_or_add_env(env_list, key, value, has_equal);
// 	free(key);
// 	return (0);
// }

/**

* Builds a new array of strings (char**) representing the 
	environment variables.
* This array is used to update the `environ` global variable 
	(or similar structure).
* Each string is in the format "KEY=VALUE".
* Returns a dynamically allocated array of strings,
	terminated by a NULL pointer.
 */
char	**build_env_array(t_env *list_env)
{
	int		count;
	char	**env_array;
	char	*joined;
	int		i;
	t_env	*tmp;

	count = count_env_items(list_env);
	env_array = malloc(sizeof(char *) * (count + 1));
	i = 0;
	if (!env_array)
		return (NULL);
	tmp = list_env;
	while (tmp)
	{
		if (tmp->content)
		{
			joined = ft_strjoin(tmp->key, "=");
			env_array[i] = ft_strjoin_free_env(joined, tmp->content);
			i++;
		}
		tmp = tmp->next;
	}
	env_array[i] = NULL;
	return (env_array);
}

/**
 * Implements the 'export' built-in command.
 * If no arguments are given, it prints all environment variables.
 * For each argument, it attempts to add or update an environment variable.
 * It also rebuilds the `char** env` array after any changes.
 * Returns the status of the last export operation.
 */
int	ft_export(char **args, t_myenv *myenv)
{
	int	i;
	int	status;

	status = 0;
	if (!args[1])
	{
		print_export(myenv->list_env);
		return (0);
	}
	i = 1;
	while (args[i])
	{
		status = export_add_or_update(&myenv->list_env, args[i]);
		i++;
	}
	free_env_array(myenv->env);
	myenv->env = build_env_array(myenv->list_env);
	return (status);
}
