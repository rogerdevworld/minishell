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

int	export_add_or_update(t_env **env_list, char *arg)
{
	char	*key;
	char	*value;
	int		has_equal;

	if (!parse_export_arg(arg, &key, &value, &has_equal))
		return (1);
	if (!is_valid_identifier(key))
	{
		print_invalid_identifier_error(arg);
		free(key);
		free(value);
		return (1);
	}
	update_or_add_env(env_list, key, value, has_equal);
	free(key);
	return (0);
}

/**
 * para ir actualizando char **env cada vez que se ejecuta export
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
