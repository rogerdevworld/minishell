/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 21:49:28 by rmarrero          #+#    #+#             */
/*   Updated: 2025/06/29 17:35:00 by xviladri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/minishell.h"

void	process_line(char *line, char **envp, t_myenv *myenv, t_token *tokens)
{
	t_command	*cmd;	

	tokens = lexer(line);
	cmd = parse_tokens(tokens, envp);
	ft_check_executor(cmd, envp, myenv);
	free_tokens(tokens);
	free_command_list(cmd);
	free(line);
}

// -- main loop for minishell -- //
void	main_loop(char **envp)
{
	char		*line;
	t_myenv		*myenv;
	t_token		*tokens;

	tokens = NULL;
	myenv = ft_myenv(envp);
	while (1)
	{
		line = readline(meta_path(envp));
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
		process_line(line, envp, myenv, tokens);
	}
	exit(0);
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
	main_loop(envp);
	return (0);
}
/*// -- commad for test -- //
ls -la > text && cat -e text && echo "holo world" > new_text && cat text
&& echo hola && cat < new_text || ls -la > text && wc - l text > text_c
&& cat text*
*/

// valgrind --leak-check=full --show-leak-kinds=all ./minishell
// valgrind --leak-check=full ./minishell

// -- prompt inicial parser manejar "> < << | || &&"
// -- pwd | cat -e | cat -e
// -- ahora
// -- t_cmd 1. pwd t_cmd 2. cat -e t_cmd 3. cat -e
// -- flags | || &&

// -- pipe
// -- t_bonus 1. flag pipe
// -- -- t_cmd 1. pwd
// -- t_bonus 2. flag pipe
// -- -- t_cmd 2. cat -e
// -- t_bonus 3. flag pipe
// -- -- t_cmd 3. cat -e

// -- &&, || or (both)
// -- pwd > text && cat -e text || pwd > text | cat -e text
// -- t_bonus 1. && first element
// -- -- t_cmd 1. pwd > text
// -- t_bonus 2. rigth pwd, left cat -e
// -- -- t_cmd 2. cat -e
// -- t_bonus 3. rigth
// -- -- t_cmd 3. cat -e*/
