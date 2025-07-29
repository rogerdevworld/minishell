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

void	print_invalid_identifier_error(char *arg)
{
	ft_putstr_fd("export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}

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
		// AÑADIDO: Liberar 'value' después de que ft_env_new lo duplique.
		// Esto corrige el leak de 'definitely lost'.
		if (value)
			free(value);
	}
}