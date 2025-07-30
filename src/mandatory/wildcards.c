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

/**
 * @brief Processes a single argument for wildcard expansion.
 * This function checks if the given argument contains a wildcard character '*'.
 * If it does, it attempts to expand the wildcard. Otherwise, it duplicates
 * the argument and adds it to the new arguments array.
 * @param arg The argument string to process.
 * @param new_args A pointer to the array of new arguments. 
	This array will be
 * updated with expanded arguments or the original argument.
 * @param count A pointer to an integer representing the 
	current count of arguments
 * in `new_args`. This count is incremented if an argument is added.
 */
static void	process_argument(char *arg, char ***new_args, int *count)
{
	if (ft_strchr(arg, '*'))
		expand_single_wildcard(arg, new_args, count);
	else
		*new_args = add_arg(*new_args, count, ft_strdup(arg));
}

/**
 * @brief Expands wildcard characters in an array of arguments.
 *
 * This function iterates through an array of arguments and performs wildcard
 * expansion for any argument containing a '*'. The original arguments array
 * is freed and replaced with a new array containing the expanded arguments.
 *
 * @param args A pointer to a pointer to a null-terminated array of strings.
 * This array will be modified in place to contain the arguments
 * after wildcard expansion.
 */
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
