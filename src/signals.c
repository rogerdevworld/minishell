/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:05:47 by rmarrero          #+#    #+#             */
/*   Updated: 2025/04/22 12:42:00 by xviladri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/minishell.h"

int			g_signal;

void	set_defaul_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

static int	handle_normal_exit(int status)
{
	return WEXITSTATUS(status);
}

static int	handle_signal_exit(int status)
{
	int signo = WTERMSIG(status);

	if (signo == SIGPIPE)
		return 0;
	else if (signo == SIGINT)
	{
		write(1, "\n", 1);
		return 130;
	}
	else if (signo == SIGQUIT)
	{
		write(1, "Quit (core dumped)\n", 19);
		return 131;
	}
	return (128 + signo);
}

int	update_exit_status(int status, t_minishell *minishell)
{
	if (WIFEXITED(status))
		status = handle_normal_exit(status);
	else if (WIFSIGNALED(status))
		status = handle_signal_exit(status);

	if (g_signal == S_SIGINT_CMD)
		status = 130;

	g_signal = S_BASE;

	if (minishell)
		minishell->exit = status;

	return status;
}

static void	sigint_handler_aux(void)
{
	if (g_signal == S_HEREDOC_END)
	{
		ft_putstr_fd("\n", 1);
		g_signal = S_CANCEL_EXEC;
	}
	else if (g_signal == S_BASE || g_signal == S_HEREDOC)
		g_signal = S_SIGINT;
}

static void	sigint_handler(int sig)
{
	(void)sig;
	if (g_signal == S_BASE || g_signal == S_SIGINT)
	{
		rl_on_new_line();
		rl_redisplay();
		ft_putstr_fd("\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	else if (g_signal == S_CMD)
	{
		g_signal = S_SIGINT_CMD;
		ft_putstr_fd("\n", 1);
		rl_on_new_line();
	}
	else if (g_signal == S_HEREDOC)
	{
		ioctl(0, TIOCSTI, '\n');
		exit(0);
	}
	sigint_handler_aux();
}

void	signal_init(void)
{
	g_signal = S_BASE;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}