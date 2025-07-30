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

/**
 * @brief Global variable to track the current signal state.
 *
 * This variable is used to manage different signal handling behaviors
 * based on the current operational context of the minishell (e.g., base prompt,
 * command execution, heredoc input).
 */
int		g_signal;

/**

 * @brief Determines the exit status based on the signal 
	that terminated a child process.
 * This function is called when a child process terminates due to a signal.
 * It translates common signals (SIGPIPE, SIGINT, SIGQUIT) into specific
 * exit statuses recognized by the shell.
 *
 * @param status The status returned by `waitpid()`.
 * @return The appropriate exit status for the shell.
 */
int	handle_signal_exit(int status)
{
	int	signo;

	signo = WTERMSIG(status);
	if (signo == SIGPIPE)
		return (0);
	else if (signo == SIGINT)
		return (130);
	else if (signo == SIGQUIT)
	{
		write(1, "Quit (core dumped)\n", 19);
		return (131);
	}
	return (128 + signo);
}

/**

 * @brief Updates the minishell's exit status based on a 
 	process's termination status.
 * This function takes the raw status from `waitpid()` and determines the
 * correct exit code. It distinguishes between normal exits 
	and exits due to signals,
 * updating the global `g_signal` and the `minishell` structure accordingly.
 *
 * @param status The status returned by `waitpid()`.
 * @param minishell A pointer to the minishell structure,
	where the exit status will be stored.
 * @return The calculated exit status.
 */
int	update_exit_status(int status, t_minishell *minishell)
{
	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		status = handle_signal_exit(status);
	if (g_signal == S_SIGINT_CMD)
		status = 130;
	g_signal = S_BASE;
	if (minishell)
		minishell->exit = status;
	return (status);
}

/**
 * @brief Auxiliary function for SIGINT handler.
 * This function handles specific behaviors for SIGINT 
 	based on the current
 * `g_signal` state, particularly for heredoc termination.
 */
void	sigint_handler_aux(void)
{
	if (g_signal == S_HEREDOC_END)
	{
		ft_putstr_fd("\n", 1);
		g_signal = S_CANCEL_EXEC;
	}
	else if (g_signal == S_BASE || g_signal == S_HEREDOC)
		g_signal = S_SIGINT;
}

/**
 * @brief Signal handler for SIGINT (Ctrl+C).
 *
 * This function defines the behavior of the shell when a 
	SIGINT signal is received.
 * It adjusts `readline`'s display, prints newlines, and updates the global
 * `g_signal` variable based on the context (base, command execution,
	or heredoc).
 * @param sig The signal number (expected to be SIGINT).
 */
void	ft_sigint(int sig)
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
		ft_putstr_fd("\n", 2);
		rl_on_new_line();
	}
	else if (g_signal == S_HEREDOC)
	{
		g_signal = S_SIGINT;
		ft_putstr_fd("\n", 2);
		rl_on_new_line();
	}
	sigint_handler_aux();
}

/**
 * @brief Initializes the signal handlers for the minishell.
 *
 * This function sets the global `g_signal` to its base state and configures
 * the signal handlers for SIGINT (Ctrl+C) and SIGQUIT (Ctrl+\).
 * SIGINT is handled by `ft_sigint`,
	while SIGQUIT is ignored to prevent core dumps.
 */
void	signal_init(void)
{
	g_signal = S_BASE;
	signal(SIGINT, ft_sigint);
	signal(SIGQUIT, SIG_IGN);
}
