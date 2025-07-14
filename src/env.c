/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:05:47 by rmarrero          #+#    #+#             */
/*   Updated: 2025/04/09 23:48:01 by xviladri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/minishell.h"

t_env *ft_env_new(char *key, char *content)
{
	t_env *new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = ft_strdup(key);
	if (!content)
		new->content = NULL;
	else
		new->content = ft_strdup(content);
	new->next = NULL;
	return (new);
}

void ft_env_add_back(t_env **lst, t_env *new)
{
	t_env *tmp;

	if (!lst || !new)
		return;
	if (!*lst)
	{
		*lst = new;
		return;
	}
	tmp = *lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

void ft_env(t_env **env_list, char **env)
{
	int i;
	char *sep;
	t_env *new;
	char *key;
	char *value;

	i = 0;
	while (env[i])
	{
		sep = ft_strchr(env[i], '=');
		if (sep)
		{
			key = ft_substr(env[i], 0, sep - env[i]);
			value = ft_strdup(sep + 1);
			new = ft_env_new(key, value);
			ft_env_add_back(env_list, new);
			free(key);
			free(value);
		}
		i++;
	}
}
void print_env(t_myenv *myenv)
{
	t_env *tmp;

	if (!myenv || !myenv->list_env)
		return;
	tmp = myenv->list_env;
	while (tmp)
	{
		if (tmp->content)
			ft_printf("%s=%s\n", tmp->key, tmp->content);
		tmp = tmp->next;
	}
}

t_myenv *ft_myenv(char **env)
{
	t_myenv *myenv;
	char cwd[PATH_MAX];

	myenv = malloc(sizeof(t_myenv));
	if (!myenv)
		return (NULL);
	myenv->env = ft_dup_env(env);
	myenv->list_env = NULL;
	ft_env(&myenv->list_env, env);
	if (!env_has_key(myenv->list_env, "PATH"))
		ft_add_env(myenv, "PATH", "/usr/local/bin:/usr/bin:/bin");
	if (!env_has_key(myenv->list_env, "PWD") && getcwd(cwd, sizeof(cwd)))
		ft_add_env(myenv, "PWD", cwd);
	if (!env_has_key(myenv->list_env, "SHLVL"))
		ft_add_env(myenv, "SHLVL", "1");
	if (!env_has_key(myenv->list_env, "_"))
		ft_add_env(myenv, "_", "/usr/bin/env");
	return (myenv);
}

void ft_sort_export(t_env *env)
{
	t_env *ini;
	char *tmp;

	ini = env;
	if (!env)
		return;
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

char **ft_dup_env(char **envp)
{
	int i = 0;
	char **copy;

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
	return copy;
}

void ft_add_env(t_myenv *myenv, const char *key, const char *value)
{
	char *arg;
	int len_key = ft_strlen(key);
	int len_val = ft_strlen(value);

	arg = malloc(len_key + len_val + 2);
	if (!arg)
		return;
	strcpy(arg, key);
	arg[len_key] = '=';
	strcpy(arg + len_key + 1, value);
	arg[len_key + len_val + 1] = '\0';

	export_add_or_update(&myenv->list_env, arg);
	free(arg);
}

int env_has_key(t_env *env_list, const char *key)
{
	while (env_list)
	{
		if (ft_strcmp(env_list->key, key) == 0)
			return (1);
		env_list = env_list->next;
	}
	return (0);
}
