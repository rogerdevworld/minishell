/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 20:41:27 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/16 20:41:34 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

// Lista de operadores reconocidos
int	is_operator(const char *s)
{
	return (ft_strncmp(s, "&&", 2) == 0 || ft_strncmp(s, "||", 2) == 0
		|| ft_strncmp(s, ">>", 2) == 0 || ft_strncmp(s, "<<", 2) == 0
		|| *s == '|' || *s == '<' || *s == '>' || *s == '(' || *s == ')');
}

// Extrae un operador del input
int	read_operator(const char *s, char **out)
{
	if (ft_strncmp(s, "&&", 2) == 0 || ft_strncmp(s, "||", 2) == 0
		|| ft_strncmp(s, ">>", 2) == 0 || ft_strncmp(s, "<<", 2) == 0)
	{
		*out = ft_substr(s, 0, 2);
		return (2);
	}
	else
	{
		*out = ft_substr(s, 0, 1);
		return (1);
	}
}

// Extrae una palabra, incluyendo comillas
int	read_word(const char *s, char **out)
{
	int		i;
	char	quote;

	i = 0;
	quote = 0;
	while (s[i] && (quote || (!ft_isspace(s[i]) && !is_operator(s + i))))
	{
		if ((s[i] == '\'' || s[i] == '"') && !quote)
			quote = s[i];
		else if (s[i] == quote)
			quote = 0;
		i++;
	}
	*out = ft_substr(s, 0, i);
	return (i);
}

// Avanza al siguiente token
void	next_token(t_token **tokens)
{
	if (*tokens)
		*tokens = (*tokens)->next;
}
