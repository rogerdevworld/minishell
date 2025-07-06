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

// -- main loop for minishell -- //
void	main_loop(char **envp)
{
	char		*line;
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
		if (g_signal == S_SIGINT)
			minishell->exit = 1;
		line = readline(ft_agnoster(envp)); //"text> "
		if (!line)
			break ;
		if (*line)	
			add_history(line);
		if (ft_strncmp(line, "exit", 4) == 0)
		{
			free(line);
			rl_free_line_state();
			rl_clear_history();
			break ;
		}
		tokens = lexer(line);
		cmd = parse_tokens(tokens, envp);
		// print_tokens(tokens);
		print_command_list(cmd);

		// Construir el AST a partir de la lista de comandos
		ast_root = build_ast(cmd);
		print_ast(ast_root, 0);
		exec = init_exec(myenv);
		minishell = init_minishell(myenv, ast_root, tokens, cmd, exec);
		// Ejecutar con el AST o con cmd según cómo tengas implementado
		if (g_signal != S_CANCEL_EXEC)
		{
			int status  = ft_check_executor(minishell, exec, cmd, envp, myenv);
			//minishell->exit = status;
			ft_printf("status executor: %i\n", status);
		}
		free_tokens(tokens);
		free_command_list(cmd);
		free_ast(ast_root);
		free(line);
	}
	g_signal = S_BASE;
}


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