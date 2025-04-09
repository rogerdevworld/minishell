/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:05:47 by rmarrero          #+#    #+#             */
/*   Updated: 2025/04/01 13:05:50 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/minishell.h"

t_env	*ft_env_new(char *key, char *content)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = ft_strdup(key);
	new->content = ft_strdup(content);
	new->next = NULL;
	return (new);
}

void	ft_env_add_back(t_env **lst, t_env *new)
{
	t_env	*tmp;

	if (!lst || !new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	tmp = *lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

void	ft_env(t_env **env_list, char **envp)
{
	int		i;
	char	*sep;
	t_env	*new;
	char	*key;
	char	*value;

	i = 0;
	while (envp[i])
	{
		sep = ft_strchr(envp[i], '=');
		if (sep)
		{
			key = ft_substr(envp[i], 0, sep - envp[i]);
			value = ft_strdup(sep + 1);
			new = ft_env_new(key, value);
			ft_env_add_back(env_list, new);
			free(key);
			free(value);
		}
		i++;
	}
}

// Agrega un nuevo par key=value a la lista de entorno
/*
void ft_env_add(t_env **env_list, const char *key, const char *value)
{
    t_env *new_node;
    
    new_node = ft_env_new(key, value);
    if (!new_node)
        return ;
    if (!*env_list)
    {
        *env_list = new_node;
        return ;
    }
    t_env *tmp = *env_list;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = new_node;
}
*/

void	print_env(t_myenv *myenv)
{
	t_env	*tmp;

	if (!myenv || !myenv->list_env)
		return ;
	tmp = myenv->list_env;
    ft_printf("local env\n");
	while (tmp)
	{
		if (tmp->content)
			ft_printf("%s=%s\n", tmp->key, tmp->content);
		tmp = tmp->next;
	}
}

t_myenv	*ft_myenv(char **envp)
{
	t_myenv	*myenv;

	myenv = malloc(sizeof(t_myenv));
	if (!myenv)
		return (NULL);
	myenv->env = NULL;
	myenv->env = NULL;
	ft_env(&myenv->list_env, envp);
	return (myenv);
}

void	free_myenv(t_myenv *myenv)
{
	if (!myenv)
		return ;
	free_env_list(myenv->list_env);
	free_env_array(myenv->env);
	free(myenv);
}


void	free_env_list(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->key);
		free(env->content);
		free(env);
		env = tmp;
	}
}
void	free_env_array(char **env_array)
{
	int	i = 0;

	if (!env_array)
		return ;
	while (env_array[i])
		free(env_array[i++]);
	free(env_array);
}