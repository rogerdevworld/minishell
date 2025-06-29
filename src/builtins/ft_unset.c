/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:05:47 by rmarrero          #+#    #+#             */
/*   Updated: 2025/06/29 18:08:01 by xviladri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/minishell.h"

static char	*ft_strjoin_3(char const *s1, char const *s2, char const *s3)
{
	char	*tmp;
	char	*res;

	if (!s1 || !s2 || !s3)
		return (NULL);
	tmp = ft_strjoin(s1, s2);
	if (!tmp)
		return (NULL);
	res = ft_strjoin(tmp, s3);
	free(tmp);
	return (res);
}

static int	count_env_vars(t_env *list_env)
{
	int	count;

	count = 0;
	while (list_env)
	{
		if (list_env->key && list_env->content)
			count++;
		list_env = list_env->next;
	}
	return (count);
}

static void	fill_env_array(t_myenv *myenv)
{
	t_env	*tmp;
	int		i;

	i = 0;
	tmp = myenv->list_env;
	while (tmp)
	{
		if (tmp->key && tmp->content)
		{
			myenv->env[i] = ft_strjoin_3(tmp->key, "=", tmp->content);
			i++;
		}
		tmp = tmp->next;
	}
	myenv->env[i] = NULL;
}

void	update_env_array(t_myenv *myenv)
{
	int	count;

	count = count_env_vars(myenv->list_env);
	if (myenv->env)
		free_env_array(myenv->env);
	myenv->env = malloc(sizeof(char *) * (count + 1));
	if (!myenv->env)
		return ;
	fill_env_array(myenv);
}

void	ft_unset(char **args, t_myenv *myenv)
{
	int	i;

	i = 1;
	if (!args || !args[0])
		return ;
	while (args[i])
	{
		if (!ft_check_wrong_char(args[i]))
			remove_env_node(&myenv->list_env, args[i]);
		i++;
	}
}
