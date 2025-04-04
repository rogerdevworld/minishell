/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <rmarrero@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:52:14 by rmarrero          #+#    #+#             */
/*   Updated: 2025/03/03 11:52:19 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

// -- function to initialize the t_cmd structure -- //
// -- apply specific operators -- //

// -- init list cmds in t_bonus -- //
t_bonus	*init_bonus(t_operator type, t_command *cmd, t_bonus *left, t_bonus *right)
{
	t_bonus	*node;

	node = malloc(sizeof(t_bonus));
	if (!node)
		return (NULL);
	node->type = type;
	node->cmd = cmd;
	node->left = left;
	node->right = right;
	return (node);
}

// -- funcion bonus() si hay operator le pasa prev gruada el token operador assig (&&, ||, |)
// y va por el next( proximo grupo de comandos) -- //

// -- resolver
t_operator	resolve_operator(char *operator)
{
	if (ft_strcmp(operator, "|") == 0)
		return (PIPE);
	if (ft_strcmp(operator, "&&") == 0)
		return (AND);
	if (ft_strcmp(operator, "||") == 0)
		return (OR);
	return (COMMAND);
}