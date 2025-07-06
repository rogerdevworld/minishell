/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:05:47 by rmarrero          #+#    #+#             */
/*   Updated: 2025/04/09 21:37:24 by xviladri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

// --- this functions is internal execve() -- xenia: para q sirve esta funcion?? No es como la de ft_check_executor??//
/*void	built(t_command *cmd, char **envp)
{
	int	i;

	envp = envp;
	while (cmd)
	{
		i = 0;
		if (get_builtin_cmd(cmd->args[i]))
		{
			execute_builtin(get_builtin_cmd(cmd->args[i]), cmd->args[i + 1], envp);
			i++;
		}
		cmd = cmd->next;
	}
}*/

int	get_builtin_cmd(char *cmd)
{
	if (ft_strcmp(cmd, "cd") == 0)
		return (CD);
	if (ft_strncmp(cmd, "exit", 5) == 0)
		return (EXIT);
	if (ft_strncmp(cmd, "echo", 4) == 0)
		return (ECHO);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (PWD);
	if (ft_strncmp(cmd, "export", 5) == 0)
		return (EXPORT);
	if (ft_strncmp(cmd, "unset", 5) == 0)
		return (UNSET);
	if (ft_strncmp(cmd, "clear", 5) == 0)
		return (CLEAR);
	if (ft_strncmp(cmd, "env", 3) == 0)
		return (ENV);
	return (-1);
}

// -- design -- //
int	execute_builtin(int cmd, char **args, char **envp, t_myenv *myenv)
{
	int status = 0;
	if (cmd == CD)
		status = ft_cd(args[1], envp);
	if (cmd == EXIT)
	{
		//ft_printf("\n PASO \n");
		status = ft_exit_builtin(args);
		ft_printf("\nel estatus de salida es: %i \n", status);
	}
	if (cmd == ECHO)
		ft_echo(args, myenv->list_env);
	if (cmd == PWD)
		status = pwd(envp);
	if (cmd == EXPORT)
		ft_export(args, myenv);
	if (cmd == UNSET)
	{
		ft_unset(args, myenv);
		update_env_array(myenv);
	}
	if (cmd == ENV)
		print_env(myenv);
	if (cmd == CLEAR)
		clear();
	//ft_printf("buitings status: %i", status);
	return (status);
}
/*
int	execute_builtin(int cmd, char **args, char **envp, t_myenv *myenv)
{
	int status = 0;
	if (cmd == CD)
		status = ft_cd(args[1], envp);
	else if (cmd == EXIT)
	{
		ft_printf("\n PASO \n");
		status = ft_exit_builtin(args);
		ft_printf("\nel estatus de salida es: %i \n", status);
	}
	else if (cmd == ECHO)
		ft_echo(args, myenv->list_env);
	else if (cmd == PWD)
		status = pwd(envp);
	else if (cmd == EXPORT)
		ft_export(args, myenv);
	else if (cmd == UNSET)
	{
		ft_unset(args, myenv);
		update_env_array(myenv);
	}
	else if (cmd == ENV)
		print_env(myenv);
	else if (cmd == CLEAR)
		clear();
	//ft_printf("buitings status: %i", status);
	return (status);
}
*/
