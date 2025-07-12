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
	//int sintax_st = 0;

	tokens = NULL;
	//status = exec->status;
	myenv = ft_myenv(envp);
	while (1)
	{
		//line = readline(ft_agnoster(envp, status));
		//line = readline(ft_strjoin("mini > ", ft_itoa(status)));
		line = readline("mini > ");
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
			//print_ast(ast, 0);
			exec = init_exec(myenv);
			minishell = init_minishell(ast, tokens, cmd, exec);
			status = execute_ast(ast, envp, myenv, minishell, status);
		}
		free(line);
	}
	g_signal = S_BASE;
} 

/**
 * old main_loop, funciona pero se cambia para manejar todo en una sola
 * estructura tipo t_minishell
 */
// -- main loop for minishell -- //
/* 
 void	main_loop(char **envp)
{
	char	*line;
	int		last_builtin_result;
	int		status;

	t_token		*tokens; // tokens first
	t_command	*cmd; // cmd secound
	t_myenv		*myenv; // env three
	t_ast_node	*ast_root; //bonus
	t_executor	*exec; //executor
	t_minishell	*minishell; //mini final
	tokens = NULL;
	myenv = ft_myenv(envp);
	status = 0;
	while (1)
	{
		//line = readline("meta_path(envp)"); //"text> "
		line = readline("minishell> "); //"text> "
		if (!line)
			break ;
		if (*line)
			add_history(line);
		//if (ft_strncmp(line, "exit", 4) == 0)
		//{
		//	last_builtin_result = execute_builtin(exec->builtin_id, cmd->args,
		//		envp, myenv);
		//	free(line);
		//	rl_free_line_state();
		//	rl_clear_history();
		//	break ;
		//}
		tokens = lexer(line);
		cmd = parse_tokens(tokens, envp);
		// Construir el AST a partir de la lista de comandos
		ast_root = build_ast(cmd); //aqui es donde esta el fallo 
		print_ast(ast_root, 0);
		exec = init_exec(myenv);
		minishell = init_minishell(myenv, ast_root, tokens, cmd, exec);
		if (g_signal != S_CANCEL_EXEC)
		{
			status  = ft_check_executor(minishell, exec, cmd, envp,
					myenv);
			//int status  = execute_astint(minishell, exec, cmd, envp, myenv);
			//status = execute_command_list(minishell, exec, cmd, envp, myenv);
			minishell->exit = status;
			ft_printf("status executor: %i\n", status);
		}
		free_tokens(tokens);
		free_command_list(cmd);
		free_ast(ast_root);
		free(line);
	}
	g_signal = S_BASE;
} */
 

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens->next;
		if (tokens->value)
			free(tokens->value);
		free(tokens);
		tokens = tmp;
	}
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