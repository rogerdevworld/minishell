/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes_from_args.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:04:07 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/15 14:04:13 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

char	**remove_quotes_from_args(char **args)
{
	int		count;
	int		i;
	int		j;
	char	**args_no_quotes;

	count = 0;
	while (args && args[count])
		count++;
	args_no_quotes = ft_calloc(count + 1, sizeof(char *));
	if (!args_no_quotes)
		return (NULL);
	i = 0;
	j = 0;
	while (i < count)
	{
		args_no_quotes[i] = remove_quotes(args[i]);
		if (!args_no_quotes[i])
		{
			while (j < i)
			{
				free(args_no_quotes[j]);
				j++;
			}
			return (free(args_no_quotes), NULL);
		}
		i++;
	}
	args_no_quotes[count] = NULL;
	return (args_no_quotes);
}
