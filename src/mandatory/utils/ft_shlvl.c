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
#include "../../../include/minishell.h"

void	ft_shlvl(t_myenv *myenv)
{
	t_env	*env;
	int		lvl;

	env = myenv->list_env;
	while (env)
	{
		if (ft_strcmp(env->key, "SHLVL") == 0)
		{
			if (env->content && ft_isdigit(env->content[0]))
			{
				lvl = ft_atoi(env->content) + 1;
				if (lvl >= 1000)
				{
					ft_printf("minishell: warning: shell level (%i) too high, \
						resetting to 1\n", lvl);
					lvl = 1;
				}
			}
			else
				lvl = 1;
			free(env->content);
			return (env->content = ft_itoa(lvl), (void)0);
		}
		env = env->next;
	}
}
