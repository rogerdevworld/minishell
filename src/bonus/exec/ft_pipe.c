/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 20:16:29 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/16 20:16:34 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../include/minishell.h"

int	execute_pipe(t_ast *node, t_myenv *myenv, t_minishell *minishell)
{
	int		fds[2];
	int		status;
	pid_t	pid1;
	pid_t	pid2;

	if (pipe(fds) == -1)
		return (1);
	g_signal = S_CMD;
	pid1 = fork();
	status = 0;
	if (pid1 == 0)
	{
		set_defaul_signals();
		close(fds[0]);
		dup2(fds[1], STDOUT_FILENO);
		exit(execute_ast(node->left, myenv, minishell, status));
	}
	pid2 = fork();
	if (pid2 == 0)
	{
		set_defaul_signals();
		close(fds[1]);
		dup2(fds[0], STDIN_FILENO);
		exit(execute_ast(node->right, myenv, minishell, status));
	}
	close(fds[0]);
	close(fds[1]);
	waitpid(pid1, &status, 0);
	update_exit_status(status, minishell);
	waitpid(pid2, &status, 0);
	return (update_exit_status(status, minishell));
}
