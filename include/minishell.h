/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:36:11 by rmarrero          #+#    #+#             */
/*   Updated: 2025/03/27 17:36:15 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// -- internal libs -- //
# include "lexer.h"
# include "../src/libft/libft.h"

// -- system.h -- //

// -- system libs -- //
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

// -- readline -- //
#include <readline/readline.h>
#include <readline/history.h>

// -- minishell.h -- //

// -- internal strcts -- //

void	main_loop(char *delimiter, int *p_fd);
void	ft_here_doc(char *delimiter);

// -- exit.h -- //
void	ft_exit(int status, char *msg);

// -- free.h -- //
void					free_split(char **split);

#endif
