/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaacosta <jaacosta@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 21:01:11 by jaacosta          #+#    #+#             */
/*   Updated: 2025/07/21 21:01:14 by jaacosta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../../include/minishell.h"

/**
 * @brief Sets default signal handlers for SIGINT and SIGQUIT.
 *
 * This function restores the default behavior for SIGINT (Ctrl+C) and
 * SIGQUIT (Ctrl+\), meaning they will terminate the process. This is typically
 * called in child processes that are executing external commands,
 * allowing those commands to handle signals as they normally would.
 */
void	set_defaul_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

/**
 * @brief Signal handler for SIGINT specifically for heredoc input.
 *
 * When a SIGINT (Ctrl+C) is received during heredoc input, this handler
 * prints a newline to the console and exits the process with a specific
 * exit code (`EXIT_SIGINT`) to indicate interruption.
 *
 * @param signo The signal number (expected to be SIGINT).
 */
static void	heredoc_sigint_handler(int signo)
{
	(void)signo;
	write(1, "\n", 1);
	exit(EXIT_SIGINT);
}

/**
 * @brief Configures signal handlers for heredoc input mode.
 *
 * This function sets up signal handling specifically when the shell is
 * reading input for a heredoc. SIGINT (Ctrl+C) is caught by 
 	`heredoc_sigint_handler`
 * to gracefully exit the heredoc input, while SIGQUIT (Ctrl+\) is ignored
 * to prevent the shell from terminating during heredoc entry.
 */
void	signals_heredoc(void)
{
	struct sigaction	sa;

	ft_bzero(&sa, sizeof(sa));
	sa.sa_handler = heredoc_sigint_handler;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}
