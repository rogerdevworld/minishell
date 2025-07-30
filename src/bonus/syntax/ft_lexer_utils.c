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
#include "../../../include/minishell.h"

/**
 * Checks if a given string starts with a recognized shell operator.
 * Operators include "&&", "||", ">>", "<<", "|", "<", ">", "&", "(", and ")".
 * Returns 1 if it's an operator, 0 otherwise.
 */
int	is_operator(const char *s)
{
	return (ft_strncmp(s, "&&", 2) == 0 || ft_strncmp(s, "||", 2) == 0
		|| ft_strncmp(s, ">>", 2) == 0 || ft_strncmp(s, "<<", 2) == 0
		|| *s == '|' || *s == '<' || *s == '>' || *s == '&' || *s == '('
		|| *s == ')');
}

/**
 * Extracts a recognized operator from the input string.
 * It handles both single-character and two-character operators.
 * Returns the length of the extracted operator.
 */
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

/**
 * Extracts a "word" from the input string, which can include quoted sections.
 * It continues reading until it encounters whitespace, an operator,
	or an unclosed quote.
 * Returns the length of the extracted word.
 */
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

/**
 * Advances the token list pointer to the next token.
 * It moves to the 'next' member of the current token, if available.
 */
void	next_token(t_token **tokens)
{
	if (*tokens)
		*tokens = (*tokens)->next;
}

/**
 * Determines the type of a token based on its string value.
 * It checks for various shell operators, redirection symbols, and parentheses.
 * If the value matches a known operator,
	it returns the corresponding token type.
 * Otherwise, it defaults to a general word token.
 * Returns an integer representing the token's type.
 */
int	set_token_type(char *value)
{
	if (!value)
		return (TOKEN_INVALID);
	else if (ft_strcmp(value, "&&") == 0)
		return (TOKEN_AND);
	else if (ft_strcmp(value, "&") == 0)
		return (TOKEN_BG);
	else if (ft_strncmp(value, "||", 2) == 0)
		return (TOKEN_OR);
	else if (ft_strncmp(value, "|", 2) == 0)
		return (TOKEN_PIPE);
	else if (ft_strncmp(value, "<<", 2) == 0)
		return (TOKEN_HEREDOC);
	else if (ft_strcmp(value, ">") == 0)
		return (TOKEN_REDIR_OUT);
	else if (ft_strcmp(value, ">>") == 0)
		return (TOKEN_APPEND);
	else if (ft_strncmp(value, "<", 1) == 0)
		return (TOKEN_REDIR_IN);
	else if (ft_strcmp(value, "(") == 0)
		return (TOKEN_OPEN_PAREN);
	else if (ft_strcmp(value, ")") == 0)
		return (TOKEN_CLOSE_PAREN);
	else
		return (TOKEN_WORD);
}
