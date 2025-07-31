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

int	verify_sigint(int status)
{
	if (g_signal == S_SIGINT)
	{
		status = 130;
		g_signal = S_BASE;
	}
	return (status);
}

static void	cleanup_loop_resources(t_minishell *minishell)
{
	if (minishell)
		free_minishell(minishell);
	g_signal = S_BASE;
}

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
