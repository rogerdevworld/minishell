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

int	read_and_prepare_line(t_loop_data *data)
{
	char	*line;

	line = readline("minishell >");
	data->status = verify_sigint(data->status);
	if (!line)
	{
		ft_putstr_fd("exit\n", 1);
		return (1);
	}
	if (*line)
		add_history(line);
	data->full_line = read_until_balanced(line);
	free(line);
	if (!data->full_line)
		return (2);
	return (0);
}

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
