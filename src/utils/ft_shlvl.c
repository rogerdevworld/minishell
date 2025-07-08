/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_shlvl.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:05:47 by rmarrero          #+#    #+#             */
/*   Updated: 2025/04/22 20:16:44 by xviladri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

void	ft_shlvl(t_minishell *minishell)
{
	t_env	*env;
	int		lvl;
	char	*new_lvl;

	env = minishell->env->list_env;
	while (env)
	{
		if (ft_strcmp(env->key, "SHLVL") == 0)
		{
			if (env->content && ft_isdigit(env->content[0]))
				lvl = ft_atoi(env->content) + 1;
			else
				lvl = 1;
			new_lvl = ft_itoa(lvl);
			if (!new_lvl)
				return ;
			free(env->content);
			env->content = new_lvl;
			return ;
		}
		env = env->next;
	}
}