/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_here_doc_expand.c                                :+:      :+:    :+:  */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:56:13 by rmarrero          #+#    #+#             */
/*   Updated: 2025/03/27 11:57:56 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static char	*append_str_segment(char *result, const char *start, int len)
{
	char	*part;

	part = ft_substr(start, 0, len);
	return (ft_strjoin_free(result, part));
}

static char	*extract_braced_var2(const char **p)
{
	const char	*start;
	int			len;

	(*p)++;
	start = *p;
	len = 0;
	while (**p && **p != '}')
	{
		(*p)++;
		len++;
	}
	if (**p == '}')
		(*p)++;
	return (ft_substr(start, 0, len));
}

char	*get_var_name(const char **p)
{
	const char	*start;
	int			len;

	if (**p == '{')
		return (extract_braced_var2(p));
	else
	{
		start = *p;
		len = 0;
		while (**p && (ft_isalnum((unsigned char)**p) || **p == '_'))
		{
			(*p)++;
			len++;
		}
		return (ft_substr(start, 0, len));
	}
}

static char	*handle_expansion(const char **p, int status)
{
	char	*var;
	char	*val;

	if (**p == '?')
	{
		(*p)++;
		return (ft_itoa(status));
	}
	if (!**p || (!ft_isalpha(**p) && **p != '_' && **p != '{'))
		return (ft_strdup("$"));
	var = get_var_name(p);
	if (var)
	{
		val = getenv(var);
		free(var);
		if (val)
			return (ft_strdup(val));
	}
	return (ft_strdup(""));
}

char	*expand_line_heredoc(const char *line, int status)
{
	char		*result;
	const char	*p;
	const char	*start;

	result = ft_strdup("");
	p = line;
	while (*p)
	{
		start = p;
		while (*p && *p != '$')
			p++;
		if (p > start)
			result = append_str_segment(result, start, p - start);
		if (*p == '$')
		{
			p++;
			result = ft_strjoin_free(result, handle_expansion(&p, status));
		}
	}
	return (result);
}
