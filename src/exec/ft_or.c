/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_or.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 20:07:29 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/16 20:07:33 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

int	execute_or(t_ast *node, t_myenv *myenv, t_minishell *minishell)
{
	int	status;

	status = 0;
	status = execute_ast(node->left, myenv, minishell, status);
	if (status != 0)
		status = execute_ast(node->right, myenv, minishell, status);
	return (status);
}
