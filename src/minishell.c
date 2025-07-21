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
	return (status);
}

/**
 * estado actual heredocs: recibe el limiter con comillas,
 * lo cual es necesario para hacer la validacion de si es expandible o no
 *
 */
/**
 * nueva version de main_loop
 */
// line = readline(ft_agnoster(envp, status));
// line = readline(ft_agnoster(envp, status));
// line = readline(ft_strjoin("mini > ", ft_itoa(status)));
void	main_loop(t_myenv *myenv)
{
	char		*line;
	t_token		*tokens;
	t_ast		*ast;
	//t_executor	*exec;
	t_minishell	*minishell;
	int			status;

	status = 0;
	while (1)
	{
		line = readline("mini > ");
		// line = readline(ft_desing(myenv->env, status));
		status = verify_sigint(status);
		if (!line)
			break ;
		if (*line)
			add_history(line);
		tokens = lexer(line);
		// print_tokens(tokens);
		if (check_multiple_expansions_at_start(tokens, myenv->list_env))
		{
			status = 1;
			free(line);
			free_tokens(tokens); // libera tokens si corresponde
			continue ;
		}
		expand_before_executor(&tokens, myenv->list_env, status);
		shift_empty_tokens(&tokens);
		if (validate_syntax(tokens) || check_unclosed_quotes(line))
		{
			status = 2;
			free(line);
			free_tokens(tokens);
			continue ;
		}
		// PARSEAMOS LOS TOKENS A AST
		ast = parse_expression(&tokens, myenv->env);
		// /print_ast(ast, 0);
		// Procesamos los heredocs una sola vez, ya con el AST listo
		if (preprocess_heredocs(ast, status) == -1)
		{
			// interrumpido con Ctrl+C en algún heredoc
			status = 130;
			ft_destroyer(minishell);
			free(line);
			continue ; // vuelve al prompt sin ejecutar nada
		}
		else if (ft_strcmp(line, "./minishell") == 0)
        {
            ft_shlvl(myenv);
            clear_history();
        }
		else
		{
			//printf("%i\n", preprocess_heredocs(ast));
			//exec = init_exec(myenv);
			minishell = init_minishell(ast, tokens, myenv);
			if (g_signal != S_CANCEL_EXEC)
				status = execute_ast(ast, myenv->env, myenv, minishell, status);
			update_exit_status(status, minishell);
		}
		ft_destroyer(minishell);
		free(line);
		if (g_signal == SIGQUIT)
			status = 0;
		g_signal = S_BASE;
	}

	// g_signal = S_BASE;
	//status = 0; 
}

// void main_loop(t_myenv *myenv)
// {
// 	char *line;
// 	t_token *tokens;
// 	t_ast *ast;
// 	t_executor *exec;
// 	t_minishell *minishell;
// 	int status;

// 	status = 0;
// 	tokens = NULL;
// 	while (1)
// 	{
// 		line = readline("mini > ");
// 		//line = readline(ft_desing(myenv->env, status));
// 		status = verify_sigint(status);
// 		if (!line)
// 			break ;
// 		if (*line)
// 			add_history(line);
// 		tokens = lexer(line);
// 		if (check_multiple_expansions_at_start(tokens, myenv->list_env))
// 		{
// 			status = 1;
// 			continue ;
// 		}
// 		expand_before_executor(&tokens, myenv->list_env, status);
// 		shift_empty_tokens(&tokens);
// 		if (validate_syntax(tokens) || check_unclosed_quotes(line))
// 			status = 2;
// 		else if (ft_strcmp(line, "./minishell") == 0)
//         {
//             ft_shlvl(myenv);
//             clear_history();
//         }
//         else
//         {
// 			//print_tokens(tokens);
// 			ast = parse_expression(&tokens, myenv->env);
// 			//print_ast(ast, 0);
// 			exec = init_exec(myenv);
// 			minishell = init_minishell(ast, tokens, exec);
// 			if (g_signal != S_CANCEL_EXEC)
// 				status = execute_ast(ast, myenv->env, myenv, minishell, status);
// 		}
// 		//ft_destroyer(minishell);
// 		free(line);
// 	}
// 	g_signal = S_BASE;
// }

int	main(int argc, char **argv, char **env)
{
	t_myenv *myenv;

	(void)argc;
	(void)argv;

	myenv = ft_myenv(env);
	signal_init();
	main_loop(myenv);
	return (0);
}