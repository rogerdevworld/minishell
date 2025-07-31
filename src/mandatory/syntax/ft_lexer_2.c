/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexer_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 20:41:09 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/16 20:41:15 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../include/minishell.h"

/**
 * Removes "empty" tokens (tokens with NULL or empty string values)
	from a linked list of tokens.
 * It iterates through the list, freeing and unlinking any empty tokens.
 */
static void	remove_token(t_token **head, t_token **tmp, t_token **prev)
{
	t_token	*to_delete;

	to_delete = *tmp;
	if (*prev == NULL)
	{
		*head = (*tmp)->next;
		*tmp = *head;
	}
	else
	{
		(*prev)->next = (*tmp)->next;
		*tmp = (*tmp)->next;
	}
	if (to_delete->value)
		free(to_delete->value);
	free(to_delete);
}

void	shift_empty_tokens(t_token **head)
{
	t_token	*tmp;
	t_token	*prev;

	tmp = *head;
	prev = NULL;
	while (tmp)
	{
		if (!tmp->value || tmp->value[0] == '\0')
			remove_token(head, &tmp, &prev);
		else
		{
			prev = tmp;
			tmp = tmp->next;
		}
	}
}

/**
 * Reads lines from standard input until parentheses are balanced.
 * It first analyzes an initial line, and if parentheses are unbalanced,
 * it prompts the user for more input until balance is achieved or 
 	EOF is reached.
 * Returns a single concatenated string of all read lines,
	or NULL on an invalid balance.
 */
static int	append_lines_until_balanced(char **full_line)
{
	char	*line;
	char	*tmp;
	t_token	*tokens;
	int		balance;

	while (1)
	{
		tokens = lexer(*full_line);
		balance = check_parentheses_balance(tokens);
		free_tokens(tokens);
		if (balance < 0)
			return (free(*full_line), 1);
		else if (balance == 0)
			break ;
		line = readline("> ");
		if (!line)
			break ;
		tmp = *full_line;
		*full_line = ft_strjoin(tmp, line);
		free(tmp);
		free(line);
	}
	return (0);
}

char	*read_until_balanced(char *initial_line)
{
	char	*full_line;

	full_line = ft_strdup(initial_line);
	if (!full_line)
		return (NULL);
	if (append_lines_until_balanced(&full_line) != 0)
		return (NULL);
	return (full_line);
}
