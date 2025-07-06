/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 17:34:46 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/03 17:34:48 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

t_executor	*init_exec(t_myenv *myenv)
{
	t_executor	*exec;

	if (!myenv)
		return (NULL);
	exec = malloc(sizeof(t_executor));
	if (!exec)
		return (NULL);
	exec->status = 0;
	exec->p_fd[0] = -1;
	exec->p_fd[1] = -1;
	exec->prev_fd = -1;
	exec->pid = -1;
	exec->builtin_id = -1;
	exec->saved_stdin = -1;
	exec->saved_stdout = -1;
	exec->myenv = myenv;
	exec->envp = myenv->env;
	return (exec);
}
