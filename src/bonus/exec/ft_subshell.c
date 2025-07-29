/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_subshell.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 19:59:26 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/16 19:59:32 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../include/minishell.h"

int	execute_subshell(t_ast *node, t_myenv *myenv, t_minishell *minishell)
{
	pid_t	pid;
	int		status;

	g_signal = S_CMD;
	pid = fork();
	status = 0;
	if (pid == 0)
	{
		set_defaul_signals();
		exit(execute_ast(node->left, myenv, minishell, status));
	}
	waitpid(pid, &status, 0);
	status = update_exit_status(status, minishell);
	return (status);
}
