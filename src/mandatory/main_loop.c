/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:56:13 by rmarrero          #+#    #+#             */
/*   Updated: 2025/04/09 22:22:59 by xviladri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/minishell.h"

/**
 * @brief Verifies and handles SIGINT status.
 *
 * This function checks if a SIGINT (Ctrl+C) was received. If so, it sets the
 * status to 130 (conventional exit status for SIGINT) and resets the global
 * signal state to base.
 *
 * @param status The current exit status.
 * @return The updated exit status, potentially 130 if SIGINT was caught.
 */
static int	verify_sigint(int status)
{
	if (g_signal == S_SIGINT)
	{
		status = 130;
		g_signal = S_BASE;
	}
	return (status);
}

/**
 * @brief Cleans up resources allocated during the main loop iteration.
 *
 * This function frees the `minishell` structure if it exists and resets
 * the global signal state to base, preparing for the next loop iteration.
 *
 * @param minishell A pointer to the minishell structure to be freed.
 */
static void	cleanup_loop_resources(t_minishell *minishell)
{
	if (minishell)
		free_minishell(minishell);
	g_signal = S_BASE;
}

/**
 * @brief The main loop of the minishell program.
 *
 * This function continuously reads user input, processes it, 
 	and executes commands.
 * It handles signal interruptions, tokenization, parsing, and execution,
 * along with resource cleanup for each iteration. The loop continues until
 * `read_and_prepare_line` returns a non-zero value (e.g., EOF).
 *
 * @param myenv A pointer to the custom environment variables.
 */
void	main_loop(t_myenv *myenv)
{
	t_loop_data	data;

	data.status = 0;
	while (1)
	{
		data.status = verify_sigint(data.status);
		if (read_and_prepare_line(&data) != 0)
			break ;
		if (process_and_validate_tokens(&data, myenv) != 0)
			continue ;
		parse_and_execute(&data, myenv);
		cleanup_loop_resources(data.minishell);
	}
}
