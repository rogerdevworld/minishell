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

static int	verify_sigint(status)
{
	if (g_signal == S_SIGINT)
	{
		status = 130;
		g_signal = S_BASE;
	}
	return status;
}

/**
 * nueva version de main_loop
 */
void	main_loop(char **envp)
{
	char	*line;
	t_token *tokens;
	t_command *cmd;
	t_ast *ast;
	t_executor *exec; 
	t_myenv *myenv;
	t_minishell *minishell;
	int status = 0;

	tokens = NULL;
	myenv = ft_myenv(envp);
	while (1)
	{
		// line = readline(ft_agnoster(envp, status));	
		//line = readline(ft_agnoster(envp, status));
		//line = readline(ft_strjoin("mini > ", ft_itoa(status)));
		line = readline("mini > ");
		status = verify_sigint(status);
		if (!line)
			break ;
		if (*line)
			add_history(line);
		//sintax_st = check_unclosed_quotes(line);
		tokens = lexer(line);
		if (validate_syntax(tokens) || check_unclosed_quotes(line))
			status = 2;
		else
		{
			///print_tokens(tokens);
			ast = parse_expression(&tokens, envp);
			///print_ast(ast, 0);
			exec = init_exec(myenv);
			minishell = init_minishell(ast, tokens, cmd, exec);
			if (g_signal != S_CANCEL_EXEC)
				status = execute_ast(ast, envp, myenv, minishell, status);
		}
		free(line);
	}
	g_signal = S_BASE;
}
// -- structuracion del codigo para el main -- //
/*
	1. inicializacion de las structs para el shell
		1.1 t_shell
	2. prompt para el path in input
	3. bucle main
	4. free
*/
int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;

	// -- inicializacion de las funciones -- //
	// -- senales -- //
	signal_init();
	/*
	signal(SIGINT, ft_sigint);
	signal(SIGQUIT, ft_sigquit);
	*/
	// -- main loop -- //
	main_loop(envp);
	return (0);
}


/*
ls && ls -la && (echo "adios" || echo hola) && ls -la | cat -e && pwd > text && cat -e < text
*/