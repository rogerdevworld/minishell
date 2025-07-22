/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:56:13 by rmarrero          #+#    #+#             */
/*   Updated: 2025/03/27 11:57:56 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef SIGNALS_H
# define SIGNALS_H

typedef enum e_signal
{
	S_BASE,
	S_HEREDOC,
	S_HEREDOC_END,
	S_SIGINT,
	S_SIGINT_CMD,
	S_SIGINT_HEREDOC,
	S_CMD,
	S_CANCEL_EXEC,
	S_SIZE
}		t_signal;

// -- signals.h -- //
void	signal_init(void);
void	ft_sigint(int sig);
void	signals_heredoc(void);
int		update_exit_status(int status, t_minishell *minishell);
void	set_defaul_signals(void);

#endif