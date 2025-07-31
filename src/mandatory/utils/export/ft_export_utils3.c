/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaacosta <jaacosta@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 01:15:50 by jaacosta          #+#    #+#             */
/*   Updated: 2025/07/22 01:15:52 by jaacosta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/minishell.h"

static void	update_existing_var(t_env *node, char *value, int mode)
{
	char	*new_val;

	if (mode == 1 && node->content)
	{
		new_val = ft_strjoin(node->content, value);
		free(node->content);
		node->content = new_val;
		free(value);
	}
	else if (value)
	{
		free(node->content);
		node->content = value;
	}
	else
		free(value);
}

void	update_or_add_env(t_env **env_list, char *key, char *value, int mode)
{
	t_env	*node;

	node = find_env_var(*env_list, key);
	if (node)
		update_existing_var(node, value, mode);
	else
	{
		ft_env_add_back(env_list, ft_env_new(key, value));
		if (value)
			free(value);
	}
}
