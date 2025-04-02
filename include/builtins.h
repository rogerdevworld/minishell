/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:56:13 by rmarrero          #+#    #+#             */
/*   Updated: 2025/03/27 11:57:56 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef BUILTINS_H
# define BUILTINS_H

// -- lexer.h -- //

// --- internal commads -- //
enum
{
	CD,
	EXIT,
	ECHO,
	PWD,
	EXPORT,
	UNSET,
	CLEAR,
	NUM_BUILTINS
};

// -- internal to execve() -- //
void	built(t_command *cmd, t_token *token);

int		get_builtin_cmd(char *cmd);
void	execute_builtin(int cmd, t_token *know_token);
void	clear_screen(void);

// -- local commands -- //
void	pwd(void);
void	clear(void);

#endif