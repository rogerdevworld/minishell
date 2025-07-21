/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaacosta <jaacosta@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 21:26:14 by jaacosta          #+#    #+#             */
/*   Updated: 2025/07/21 21:26:16 by jaacosta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../include/minishell.h"

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*joined;

	joined = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (joined);
}

char	*ft_echo_expand(char *str, t_env *env)
{
	while (env)
	{
		if (ft_strcmp(str, env->key) == 0)
			return (ft_strdup(env->content));
		env = env->next;
	}
	return (NULL);
}

int	is_valid_n_flag(char *str)
{
	int	i;

	if (!str || str[0] != '-')
		return (0);
	i = 1;
	if (str[i] == '\0')
		return (0);
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

char	*copy_plain_text(char *arg, int *i)
{
	int	start;

	start = *i;
	while (arg[*i] && arg[*i] != '$' && arg[*i] != '\'' && arg[*i] != '"'
		&& arg[*i] != '\\')
		(*i)++;
	return (ft_substr(arg, start, *i - start));
}

char	*copy_single_quoted_text(const char *arg, int *i)
{
	char	*text;
	int		start;

	start = ++(*i);
	while (arg[*i] && arg[*i] != '\'')
		(*i)++;
	text = ft_substr(arg, start, *i - start);
	if (arg[*i] == '\'')
		(*i)++;
	return (text);
}
