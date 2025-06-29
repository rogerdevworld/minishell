/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xviladri <xviladri@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 18:03:31 by xviladri          #+#    #+#             */
/*   Updated: 2025/06/29 18:04:21 by xviladri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/minishell.h"

void	put_error(char *prefix, char *cmd, char *msg)
{
	ft_printf("%s: %s: %s\n", prefix, cmd, msg);
}

int	ft_check_wrong_char(char *s)
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

void	remove_env_node(t_env **env_list, const char *key)
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
