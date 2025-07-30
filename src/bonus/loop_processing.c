/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_processing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:56:13 by rmarrero          #+#    #+#             */
/*   Updated: 2025/04/09 22:22:59 by xviladri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/minishell.h"

/**
 * @brief Reads a line of input from the user and prepares it for processing.
 *
 * This function prompts the user with "minishell >", reads their input,
 * adds it to the history, and then processes it to ensure parentheses
 * and quotes are balanced.
 *
 * @param data A pointer to a `t_loop_data` structure which will store the
 * processed full line of input.
 * @return Returns 0 on success, 1 if `readline` returns NULL (e.g., EOF),
 * or 2 if `read_until_balanced` fails (e.g., due to an unclosed quote).
 */
int	read_and_prepare_line(t_loop_data *data)
{
	char	*line;

	line = readline("minishell >");
	if (!line)
		return (1);
	if (*line)
		add_history(line);
	data->full_line = read_until_balanced(line);
	free(line);
	if (!data->full_line)
		return (2);
	return (0);
}

/**
 * @brief Processes and validates the tokens generated from the input line.
 *
 * This function takes the raw input line, converts it into a list of tokens,
 * performs various expansions (e.g., environment variables), 
 	handles empty tokens,
 * and validates the overall syntax and quote balancing.
 * @param data A pointer to a `t_loop_data` structure containing 
 	the full input line
 * and where the processed tokens and updated status will be stored.
 * @param myenv A pointer to the custom environment variables used for expansion.
 * @return Returns 0 on successful processing and validation, or 1 if any
 * validation or expansion check fails.
 */
int	process_and_validate_tokens(t_loop_data *data, t_myenv *myenv)
{
	data->tokens = lexer(data->full_line);
	if (check_multiple_expansions_at_start(data->tokens, myenv->list_env))
	{
		data->status = 1;
		free(data->full_line);
		free_tokens(data->tokens);
		return (1);
	}
	expand_before_executor(&data->tokens, myenv->list_env, data->status);
	shift_empty_tokens(&data->tokens);
	if (validate_syntax(data->tokens) || check_unclosed_quotes(data->full_line))
	{
		data->status = 2;
		free(data->full_line);
		free_tokens(data->tokens);
		return (1);
	}
	return (0);
}
