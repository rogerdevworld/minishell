/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:05:47 by rmarrero          #+#    #+#             */
/*   Updated: 2025/04/22 20:30:32 by xviladri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/minishell.h"

void	put_error(char *prefix, char *cmd, char *msg)
{
	ft_printf("%s: %s: %s\n", prefix, cmd, msg);
}

static int	ft_check_wrong_char(char *s)
{
	int	i;

	i = 0;
	if (!s || !s[0] || ft_isdigit(s[0]) || s[0] == '=')
	{
		put_error("bash: unset", s, "not a valid identifier");
		return (1);
	}
	while (s[i])
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
		{
			put_error("bash: unset", s, "not a valid identifier");
			return (1);
		}
		i++;
	}
	return (0);
}

static void	remove_env_node(t_env **env_list, const char *key)
{
	t_env	*curr;
	t_env	*prev;

	curr = *env_list;
	prev = NULL;
	while (curr)
	{
		if (ft_strcmp(curr->key, key) == 0)
		{
			if (prev)
				prev->next = curr->next;
			else
				*env_list = curr->next;
			free(curr->key);
			free(curr->content);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

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

void	update_env_array(t_myenv *myenv)
{
	t_env	*tmp;
	int		count;
	int		i;

	count = 0;
	tmp = myenv->list_env;
	while (tmp)
	{
		if (tmp->key && tmp->content)
			count++;
		tmp = tmp->next;
	}
	if (myenv->env)
		free_env_array(myenv->env);
	myenv->env = malloc(sizeof(char *) * (count + 1));
	if (!myenv->env)
		return ;
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
