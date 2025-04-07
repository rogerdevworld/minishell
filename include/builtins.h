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
	ENV
};

// -- internal to execve() -- //
void	built(t_command *cmd, char **envp);
int		get_builtin_cmd(char *cmd);
void	execute_builtin(int cmd, char **args, char **envp);

// -- local commands -- //
void	pwd(void);
void	clear(void);
void	ft_cd(const char *path, char **envp);
void    ft_exit_builtin(void);
void    ft_echo(char **args);
void    ft_export(char **args, char **envp);
int     ft_unset(char **args, char **envp);
static int      ft_unset_strcmp(char *s, char **envp);
static int      ft_check_wrong_char(char *s);
int get_next_quote(int start, char *str, char c);
void    put_error(char *prefix, char *cmd, char *msg);

#endif
