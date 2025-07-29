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
#include "../include/minishell.h"

t_minishell	*init_minishell(t_ast *ast, t_myenv *myenv, char *full_line)
{
	t_minishell	*minishell;

	minishell = malloc(sizeof(t_minishell));
	if (!minishell)
		return (NULL);
	minishell->tokens = NULL;
	minishell->ast = ast;
	minishell->myenv = myenv;
	minishell->full_line = full_line; // <--- AÑADE ESTA LÍNEA
	minishell->exit = 0;
	return (minishell);
}

// t_minishell	*init_minishell(t_ast *ast, t_token *tokens, t_myenv *myenv)
// {
// 	t_minishell	*minishell;

// 	minishell = malloc(sizeof(t_minishell));
// 	if (!minishell)
// 		return (NULL);
// 	minishell->tokens = tokens;
// 	minishell->ast = ast;
// 	minishell->myenv = myenv;
// 	minishell->exit = 0;
// 	return (minishell);
// }
