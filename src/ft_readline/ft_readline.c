/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:05:47 by rmarrero          #+#    #+#             */
/*   Updated: 2025/04/01 13:05:50 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"
#include <readline/history.h>
#include <readline/readline.h>

static int	*history_state(void)
{
	static int	initialized = 0;

	return (&initialized);
}

static int	get_history_initialized(void)
{
	return (*history_state());
}

static void	set_history_initialized(int value)
{
	*history_state() = value;
}

char	*ft_readline(const char *prompt)
{
	char	*line;

	if (!get_history_initialized())
	{
		using_history();
		set_history_initialized(1);
	}
	line = readline(prompt);
	if (!line)
		return (NULL);
	return (line);
}

void	ft_rl_cleanup(void)
{
	if (get_history_initialized())
	{
		clear_history();
		rl_clear_history();
		rl_free_line_state();
		set_history_initialized(0);
	}
}
