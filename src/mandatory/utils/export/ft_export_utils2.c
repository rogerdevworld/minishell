/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaacosta <jaacosta@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 01:15:50 by jaacosta          #+#    #+#             */
/*   Updated: 2025/07/22 01:15:52 by jaacosta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../../include/minishell.h"

int	count_env_items(t_env *list_env)
{
	int		count;
	t_env	*tmp;

	count = 0;
	tmp = list_env;
	while (tmp)
	{
		if (tmp->content)
			count++;
		tmp = tmp->next;
	}
	return (count);
}

static const char	*find_separator(const char *arg, int *mode)
{
	const char	*p;

	*mode = 0;
	p = ft_strnstr(arg, "+=", ft_strlen(arg));
	if (p)
	{
		*mode = 1;
		return (p);
	}
	return (ft_strchr(arg, '='));
}

int	parse_export_arg(const char *arg, char **key, char **value, int *mode)
{
	const char	*separator;
	char		*temp_value;
	size_t		key_len;

	separator = find_separator(arg, mode);
	if (!separator)
	{
		*key = ft_strdup(arg);
		*value = NULL;
	}
	else
	{
		key_len = separator - arg;
		*key = ft_substr(arg, 0, key_len);
		if (*mode == 1)
			temp_value = ft_strdup(separator + 2);
		else
			temp_value = ft_strdup(separator + 1);
		*value = remove_quotes(temp_value);
		free(temp_value);
	}
	if (!*key)
		return (free(*value), 0);
	return (1);
}

void	print_invalid_identifier_error(char *arg)
{
	ft_putstr_fd("export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}
