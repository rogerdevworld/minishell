/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:05:47 by rmarrero          #+#    #+#             */
/*   Updated: 2025/04/01 13:05:50 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/minishell.h"

// -- char *getcwd(char *buf, size_t size); -- //
void	pwd(char **envp)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)))
		ft_printf("%s\n", cwd);
	else
	{
		chdir(ft_getenv("HOME", envp));
		perror("getcwd() error");
	}
}
