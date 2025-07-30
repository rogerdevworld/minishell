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

/**
 * @brief Updates the "SHLVL" environment variable.
 *
 * This function iterates through the environment variables to find "SHLVL".
 * If found, it increments its value. If the value is not a valid number,
 * it defaults to 1. If the incremented level is 1000 or more, it prints
 * a warning and resets the SHLVL to 1, as per standard shell behavior
 * for excessively high shell levels.
 *
 * @param myenv A pointer to the custom environment structure containing
 * the list of environment variables.
 */
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
