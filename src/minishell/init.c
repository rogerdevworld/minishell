/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 21:49:28 by rmarrero          #+#    #+#             */
/*   Updated: 2025/06/28 19:48:57 by xviladri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

t_minishell	*init_minishell(char **envp, t_token *tokens, t_command *cmd)
{
	t_minishell	*minishell;

	minishell = malloc(sizeof(t_minishell));
	if (!minishell || !envp || !tokens || !cmd)
	{
		free(minishell);
		return (NULL);
	}
	minishell->envp = envp;
	minishell->tokens = tokens;
	minishell->cmd = cmd;
	return (minishell);
}
