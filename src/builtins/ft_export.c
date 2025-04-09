/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:05:47 by rmarrero          #+#    #+#             */
/*   Updated: 2025/04/09 23:13:34 by xviladri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/minishell.h"

// -- ya esta funciona el env para aplciarle al unset y el export 
// nuestra propia versiond el env y la copia hay funcion para agregar un elemento
// voy a cear una para expandir las key a su valor algo como:
// echo $PATH salida my_getenv("PATH") y neustra lista deberia contener adicionales 
// export a=a seria como add a=a  en t_env con la funcion ft_env_add(key, value);
// al final de la lista de t_env

int	is_valid_identifier(const char *str)//funcion evita cosas tipo> export 1var=foo (q empieze por un numero).
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

void	export_add_or_update(t_env **env_list, char *arg)
{
	char	*sep = ft_strchr(arg, '=');
	char	*key;
	char	*value;
	t_env	*existing;

	if (!sep)
	{
		key = ft_strdup(arg);
		value = NULL;
	}
	else
	{
		key = ft_substr(arg, 0, sep - arg);
		value = ft_strdup(sep + 1);
	}

	if (!is_valid_identifier(key))
	{
		ft_printf("export: `%s': not a valid identifier\n", key);
		free(key);
		free(value);
		return ;
	}

	existing = find_env_var(*env_list, key);
	if (existing)
	{
		free(existing->content);
		existing->content = value;
	}
	else
	{
		ft_env_add_back(env_list, ft_env_new(key, value));
	}

	free(key);
}

void	print_export(t_env *env)
{
	if (!env)
		return ;

	ft_printf("antes");
	ft_sort_export(env);
	ft_printf("paso");
	while (env)
	{
		if (env->content)
			ft_printf("declare  x %s=\"%s\"\n", env->key, env->content);
		else
			ft_printf("declare -x %s\n", env->key);
		env=env->next;
	}
	free(env);
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
