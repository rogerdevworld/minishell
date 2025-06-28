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
#include "../../include/minishell.h"

void	sigint_handler(int signum)
{
	(void)signum;
    //char **user = NULL;

    //ft_printf("%s", meta_path(user));
    write(1, "$\n", 2);
}

void	ft_sigquit(int sig)
{
    sig = sig;
    write(1, "Quit (core dumped)\n", 20);
}
