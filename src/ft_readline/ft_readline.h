/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:56:13 by rmarrero          #+#    #+#             */
/*   Updated: 2025/03/27 11:57:56 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef FT_READLINE_H
# define FT_READLINE_H

// -- ft_readline.h -- //
typedef struct s_ShellInput
{
	char	*line;
	int		from_history;
}			t_ShellInput;

char		*ft_readline(const char *prompt);
void		ft_rl_cleanup(void);

#endif