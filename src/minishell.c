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

void	print_parser(t_command *cmd)
{
	int i;
	const char *op_strs[] = { "NONE", "PIPE", "AND", "OR" };

	while (cmd)
	{
		printf("----- Comando -----\n");

		// Mostrar los argumentos
		i = 0;
		while (cmd->args && cmd->args[i])
		{
			printf("arg[%d]: %s\n", i, cmd->args[i]);
			i++;
		}

		// Mostrar path si existe
		if (cmd->path)
			printf("path: %s\n", cmd->path);

		// Mostrar redirecciones
		if (cmd->input_file != -1)
			printf("input_file: %d\n", cmd->input_file);
		if (cmd->output_file != -1)
			printf("output_file: %d\n", cmd->output_file);

		// Mostrar heredoc
		if (cmd->limiter)
			printf("heredoc limiter: %s\n", cmd->limiter);

		// Mostrar operador
		if (cmd->operator >= 0 && cmd->operator <= 3)
			printf("operator: %s\n", op_strs[cmd->operator]);
		else
			printf("operator: UNKNOWN (%d)\n", cmd->operator);

		cmd = cmd->next;
	}
}
/**
 * nueva version de main_loop
 */
void	main_loop(char **envp)
{
	char	*line;
	int		status;
	t_token *tokens;
	t_command *cmd;
	t_ast_node *ast_root;
	t_executor *exec; 
	t_myenv *myenv;
	t_minishell *minishell;

	tokens = NULL;
	//status = exec->status;
	myenv = ft_myenv(envp);
	status = 0;
	while (1)
	{
		line = readline(ft_agnoster(envp));
		if (!line)
			break ;
		if (*line)
			add_history(line);
		tokens = lexer(line);
		//print_tokens(tokens);
		cmd = parse_tokens(tokens, envp);
		//print_parser(cmd);
		ast_root = build_ast(cmd);
		//print_ast(ast_root, 0);
		exec = init_exec(myenv);
		exec->envp = envp;
		minishell = init_minishell(ast_root, tokens, cmd, exec);
		//status = minishell->exit;
		if (g_signal != S_CANCEL_EXEC)
		{
			// Aquí puedes elegir si ejecutas AST o lista de comandos:
			// int status = execute_ast(minishell);
			//status = execute_command_list(minishell);
			status = ft_check_executor(minishell, minishell->executor, cmd, exec->envp, exec->myenv);
			minishell->exit = status;
			//ft_printf("status executor: %i\n", status);
		}
		// Liberar recursos
		free_tokens(tokens);
		free_command_list(cmd);
		free_ast(ast_root);
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
		// Ejecutar con el AST o con cmd según cómo tengas implementado
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
 

void	free_command_list(t_command *cmd)
{
	int			i;
	t_command	*tmp;

	while (cmd)
	{
		tmp = cmd->next;
		if (cmd->args)
		{
			i = 0;
			while (cmd->args[i])
			{
				free(cmd->args[i]);
				i++;
			}
			free(cmd->args);
		}
		if (cmd->path)
			free(cmd->path);
		if (cmd->limiter)
			free(cmd->limiter);
		free(cmd);
		cmd = tmp;
	}
}

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