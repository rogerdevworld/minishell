/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaacosta <jaacosta@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 01:15:50 by jaacosta          #+#    #+#             */
/*   Updated: 2025/07/22 01:15:52 by jaacosta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../../include/minishell.h"

/**
 * Counts the number of environment variables that have content (a value).
 * Iterates through the linked list of environment variables 
	and increments a counter
 * for each variable that is not just a key without an assigned value.
 * Returns the total count of such environment variables.
 */
int	count_env_items(t_env *list_env)
{
	int		count;
	t_env	*tmp;

	count = 0;
	tmp = list_env;
	while (tmp)
	{
		if (tmp->content)
			count++;
		tmp = tmp->next;
	}
	return (count);
}

/**
 * Parses an argument provided to the 'export' command.
 * It separates the argument into a key and a value, if an '=' sign is present.
 * Quotes are removed from the value part. It also indicates 
 	whether an '=' was found.
 * Returns 1 on successful parsing, 0 if memory allocation for the key fails.
 */
int	parse_export_arg(char *arg, char **key, char **value, int *has_equal)
{
	char	*sep;

	sep = ft_strchr(arg, '=');
	*has_equal = (sep != NULL);
	if (!sep)
	{
		*key = ft_strdup(arg);
		*value = NULL;
	}
	else
	{
		*key = ft_substr(arg, 0, sep - arg);
		*value = remove_quotes(sep + 1);
	}
	if (!*key)
	{
		free(*value);
		return (0);
	}
	return (1);
}

/**

 * Prints an error message to standard error indicating an invalid 
	identifier for 'export'.
 * The message format is "export: `argument`: not a valid identifier\n".
 */
void	print_invalid_identifier_error(char *arg)
{
	ft_putstr_fd("export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}

/**
 * Updates an existing environment variable or adds a new one to the list.
 * If the variable `key` already exists:
 * - If `has_equal` is true, its content is updated with the new `value`.
 * - If `has_equal` is false,
	the `value` is freed (as no new value is provided).
 * If the variable `key` does not exist,
	a new environment variable node is created and added to the list.
 */
void	update_or_add_env(t_env **env_list, char *key, char *value,
		int has_equal)
{
	t_env	*existing;

	existing = find_env_var(*env_list, key);
	if (existing)
	{
		if (has_equal)
		{
			free(existing->content);
			existing->content = value;
		}
		else
			free(value);
	}
	else
	{
		ft_env_add_back(env_list, ft_env_new(key, value));
		if (value)
			free(value);
	}
}
