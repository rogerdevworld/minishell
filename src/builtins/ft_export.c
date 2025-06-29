/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:05:47 by rmarrero          #+#    #+#             */
/*   Updated: 2025/06/29 17:50:53 by xviladri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/minishell.h"

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

static void	handle_existing_var(t_env *existing, char *value, char *sep)
{
	if (sep)
	{
		free(existing->content);
		existing->content = value;
	}
	else
		free(value);
}

static void	process_key_value(char *arg, char **key, char **value)
{
	char	*sep;

	sep = ft_strchr(arg, '=');
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
}

void	export_add_or_update(t_env **env_list, char *arg)
{
	char	*key;
	char	*value;
	t_env	*existing;

	process_key_value(arg, &key, &value);
	if (!is_valid_identifier(key))
	{
		ft_printf("export: `%s': not a valid identifier\n", arg);
		free(key);
		free(value);
		return ;
	}
	existing = find_env_var(*env_list, key);
	if (existing)
		handle_existing_var(existing, value, ft_strchr(arg, '='));
	else
		ft_env_add_back(env_list, ft_env_new(key, value));
	free(key);
}

void	ft_export(char **args, t_myenv *myenv)
{
	int	i;

	if (!args[1])
	{
		print_export(myenv->list_env);
		return ;
	}
	i = 1;
	while (args[i])
	{
		export_add_or_update(&myenv->list_env, args[i]);
		i++;
	}
}
