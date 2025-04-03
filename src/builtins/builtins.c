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

// --- this functions is internal execve() -- //
void	built(t_command *cmd, t_token *token)
{
	int	i;

	i = 0;
	while (cmd)
	{
		if (get_builtin_cmd(cmd->args[i]))
		{
			execute_builtin(get_builtin_cmd(cmd->args[i]), token);
			i++;
		}
		cmd = cmd->next;
	}
}

int	get_builtin_cmd(char *cmd)
{
	if (ft_strncmp(cmd, "cd", 2) == 0)
		return (CD);
	if (ft_strncmp(cmd, "exit", 5) == 0)
		return (EXIT);
	if (ft_strncmp(cmd, "echo", 4) == 0)
		return (ECHO);
	if (ft_strncmp(cmd, "pwd", 3) == 0)
		return (PWD);
	if (ft_strncmp(cmd, "export", 5) == 0)
		return (EXPORT);
	if (ft_strncmp(cmd, "unset", 5) == 0)
		return (UNSET);
	if (ft_strncmp(cmd, "clear", 5) == 0)
		return (CLEAR);
	return (-1);
}

void	execute_builtin(int cmd, t_token *know_token)
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
		ft_printf("Type -> %s - > %s\n", token_type_to_string(know_token->type),
			know_token->value);
}
