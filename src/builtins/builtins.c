/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:05:47 by rmarrero          #+#    #+#             */
/*   Updated: 2025/04/01 13:05:50 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

builtin_cmd	get_builtin_cmd(char *cmd)
{
	if (strcmp(cmd, "cd") == 0)
		return (CD);
	if (strcmp(cmd, "exit") == 0)
		return (EXIT);
	if (strcmp(cmd, "echo") == 0)
		return (ECHO);
	if (strcmp(cmd, "pwd") == 0)
		return (PWD);
	if (strcmp(cmd, "export") == 0)
		return (EXPORT);
	if (strcmp(cmd, "unset") == 0)
		return (UNSET);
    if (strcmp(cmd, "clear") == 0)
		return (CLEAR);
	return (-1);
}

void execute_builtin(builtin_cmd cmd, t_token *know_token)
{
    if (cmd == CD)
        ft_printf("Ejecutando cd\n");
    else if (cmd == EXIT)
        ft_printf("Ejecutando exit\n");
    else if (cmd == ECHO)
        ft_printf("Ejecutando echo\n");
    else if (cmd == PWD)
        pwd();
    else if (cmd == EXPORT)
        ft_printf("Ejecutando export\n");
    else if (cmd == UNSET)
        ft_printf("Ejecutando unset\n");
    else if (cmd == CLEAR)
        clear();
    else
        ft_printf("Type -> %s Know Token - > %s\n", token_type_to_string(know_token->type), know_token->value);
}
