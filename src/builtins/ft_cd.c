/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:05:47 by rmarrero          #+#    #+#             */
/*   Updated: 2025/04/01 13:05:50 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/minishell.h"

// -- cd .. -- //
// -- int chdir(const char *path); -- //
void	ft_cd(const char *path, char **envp)
{
	if (!path)
	{
		chdir(ft_getenv("HOME", envp));
		return ;
	}
	if (chdir(path) != 0)
	{
		ft_printf("minishell: cd %s: ", path);
		perror("");
	}
}
/*void	ft_cd(const char *path, char **envp)
{
	if (!path)//si no le pasamos una ruta (un argum), vamos a HOME
	{
		chdir(getenv("HOME"));//chdir cambia el direct de trabajo.
		return (0);
	}
	if (chdir(path) != 0)//si se le pasa una ruta, cambiamos a ella.
	{
		ft_printf("minishell: cd %s: ", path);
		perror("No such file or directory");
	}
}*/
