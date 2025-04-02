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

typedef enum
{
	CD,
	EXIT,
	ECHO,
	PWD,
	EXPORT,
	UNSET,
	CLEAR,
	NUM_BUILTINS
}			builtin_cmd;

builtin_cmd	get_builtin_cmd(char *cmd);
void		execute_builtin(builtin_cmd cmd, t_token *know_token);
void		clear_screen(void);

// -- local commands -- //
void		pwd(void);
void		clear(void);

#endif