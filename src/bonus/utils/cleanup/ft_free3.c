/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 13:35:52 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/15 13:35:55 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../../include/minishell.h"

/**
 * Frees a given pointer and sets it to NULL.
 * This is a utility function to safely deallocate memory.
 */
void	ft_clean(void *ptr)
{
	if (!ptr)
		return ;
	free(ptr);
	ptr = NULL;
}

/**
 * Frees all nodes in a linked list of environment variables.
 * It iterates through the list, freeing the key, content, 
 	and the node itself for each element.
 */
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
