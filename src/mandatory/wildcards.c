/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 21:49:28 by rmarrero          #+#    #+#             */
/*   Updated: 2025/02/22 21:49:33 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Line count: 7
static void	process_argument(char *arg, char ***new_args, int *count)
{
	if (ft_strchr(arg, '*'))
		expand_single_wildcard(arg, new_args, count);
	else
		*new_args = add_arg(*new_args, count, ft_strdup(arg));
}

// Line count: 15
void	ft_wildcards(char ***args)
{
	char	**new_args;
	int		new_arg_count;
	int		i;

	if (!*args)
		return ;
	new_args = NULL;
	new_arg_count = 0;
	i = 0;
	while ((*args)[i])
	{
		process_argument((*args)[i], &new_args, &new_arg_count);
		i++;
	}
	free_split(*args);
	*args = new_args;
}
