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
	S_BASE,        // señal Base
	S_HEREDOC,     // entra en el heredoc
	S_HEREDOC_END, // finalización del heredoc
	S_SIGINT,      // Ctrl + C
	S_SIGINT_CMD,  // Ctrl + C en medio de una comando
	S_CMD,         // se ejecuta un comando
	S_CANCEL_EXEC, // Ctrl + D en heredoc
	S_SIZE
}			t_signal;

// -- signals.h -- //
void	signal_init(void);
//void	ft_sigint(int signum);
//void	ft_sigquit(int sig);

#endif