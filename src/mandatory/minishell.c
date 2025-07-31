/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 21:49:28 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/03 14:28:07 by rmarrero         ###   ########.fr       */
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
	minishell->full_line = full_line;
	minishell->exit = 0;
	return (minishell);
}

int	main(int argc, char **argv, char **env)
{
	t_myenv	*myenv;

	(void)argc;
	(void)argv;
	myenv = NULL;
	myenv = ft_myenv(env);
	signal_init();
	ft_shlvl(myenv);
	main_loop(myenv);
	free_myenv(myenv);
	return (0);
}
