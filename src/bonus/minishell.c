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

/**
 * @brief Initializes the minishell structure.
 *
 * This function allocates memory for a `t_minishell` structure and
 * initializes its members with the provided AST, environment,
	and full input line.
 * It sets `tokens` to NULL and the initial `exit` status to 0.
 *
 * @param ast A pointer to the Abstract Syntax Tree.
 * @param myenv A pointer to the custom environment structure.
 * @param full_line The complete input line read from the user.
 * @return A pointer to the newly initialized `t_minishell` structure,
	or NULL if
 * memory allocation fails.
 */
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

/**
 * @brief The main entry point of the minishell program.
 *
 * This function initializes the environment, sets up signal handlers,
 * updates the shell level, and enters the main command loop. After the loop
 * exits, it frees the allocated environment memory.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line argument strings.
 * @param env An array of environment variables passed from the parent process.
 * @return Returns 0 on successful execution.
 */
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
