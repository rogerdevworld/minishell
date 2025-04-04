/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 21:49:28 by rmarrero          #+#    #+#             */
/*   Updated: 2025/02/22 21:49:33 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

// -- apply specific operators -- //

// -- init list cmds in t_bonus -- //
t_bonus	*init_bonus_node(t_operator type, t_command *cmd, t_bonus *left, t_bonus *right)
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