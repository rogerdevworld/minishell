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
#include "../include/minishell.h"

void	ft_wildcards(char ***args)
{
	glob_t	glob_result;
	size_t	i;
	char	**new_args;
	int		arg_count;
	int		j;

	new_args = NULL;
	arg_count = 0;
	j = 0;
	while ((*args)[j] != NULL)
	{
		if (ft_strchr((*args)[j], '*') || ft_strchr((*args)[j], '/'))
		{
			if (glob((*args)[j], 0, NULL, &glob_result) == 0)
			{
				new_args = ft_realloc(new_args, sizeof(char *) * (arg_count
							+ glob_result.gl_pathc + 1));
				if (new_args == NULL)
				{
					perror("Error allocating memory for expanded arguments");
					exit(EXIT_FAILURE);
				}
				i = 0;
				while (i < glob_result.gl_pathc)
				{
					new_args[arg_count++] = ft_strdup(glob_result.gl_pathv[i]);
					if (new_args[arg_count - 1] == NULL)
					{
						perror("Error duplicating string");
						exit(EXIT_FAILURE);
					}
					i++;
				}
				globfree(&glob_result);
			}
			else
			{
				new_args = ft_realloc(new_args, sizeof(char *) * (arg_count + 2));
				if (new_args == NULL)
				{
					perror("Error allocating memory for non-expanded arguments");
					exit(EXIT_FAILURE);
				}
				new_args[arg_count++] = ft_strdup((*args)[j]);
				if (new_args[arg_count - 1] == NULL)
				{
					perror("Error duplicating string");
					exit(EXIT_FAILURE);
				}
			}
		}
		else
		{
			new_args = ft_realloc(new_args, sizeof(char *) * (arg_count + 2));
			if (new_args == NULL)
			{
				perror("Error allocating memory for arguments");
				exit(EXIT_FAILURE);
			}
			new_args[arg_count++] = ft_strdup((*args)[j]);
			if (new_args[arg_count - 1] == NULL)
			{
				perror("Error duplicating string");
				exit(EXIT_FAILURE);
			}
		}
		j++;
	}
	new_args[arg_count] = NULL;
    j = 0;
	while ((*args)[j] != NULL)
		free((*args)[j++]);
	free(*args);
	*args = new_args;
}
