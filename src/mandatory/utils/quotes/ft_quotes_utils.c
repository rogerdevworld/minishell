/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_quotes_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaacosta <jaacosta@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 13:39:46 by jaacosta          #+#    #+#             */
/*   Updated: 2025/07/22 13:39:47 by jaacosta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../../include/minishell.h"

char	*remove_quotes(char *str)
{
	size_t	len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if ((str[0] == '"' && str[len - 1] == '"'))
		return (ft_substr(str, 1, len - 2));
	return (ft_strdup(str));
}

char	*remove_all_quotes(char *str)
{
	char	*result;
	int		i;
	int		j;
	char	quote;

	i = 0;
	j = 0;
	quote = 0;
	result = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	if (!result)
		return (NULL);
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"') && !quote)
			quote = str[i++];
		else if (str[i] == quote)
		{
			quote = 0;
			i++;
		}
		else
			result[j++] = str[i++];
	}
	return (result);
}

static char	**allocate_args_no_quotes(char **args, int *count)
{
	char	**args_no_quotes;

	*count = 0;
	while (args && args[*count])
		(*count)++;
	args_no_quotes = ft_calloc(*count + 1, sizeof(char *));
	if (!args_no_quotes)
		return (NULL);
	return (args_no_quotes);
}

char	**remove_quotes_from_args(char **args)
{
	int		count;
	int		i;
	int		j;
	char	**args_no_quotes;

	args_no_quotes = allocate_args_no_quotes(args, &count);
	if (!args_no_quotes)
		return (NULL);
	i = 0;
	j = 0;
	while (i < count)
	{
		args_no_quotes[i] = remove_all_quotes(args[i]);
		if (!args_no_quotes[i])
		{
			while (j < i)
				free(args_no_quotes[j++]);
			free(args_no_quotes);
			return (NULL);
		}
		i++;
	}
	args_no_quotes[count] = NULL;
	return (args_no_quotes);
}
