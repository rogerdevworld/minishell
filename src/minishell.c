/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 21:49:28 by rmarrero          #+#    #+#             */
/*   Updated: 2025/04/22 12:40:48 by xviladri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/minishell.h"

// -- main loop for minishell -- //
void	main_loop(char **envp)
{
	char		*line;
	t_token		*tokens;
	t_command	*cmd;
	t_myenv		*myenv;

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
		tokens = lexer(line);
		cmd = parse_tokens(tokens, envp);
		ft_check_executor(cmd, envp, myenv);
		free_tokens(tokens);
		free_command_list(cmd);
		free(line);
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
int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, ft_sigquit);
	main_loop(envp);
	return (0);
}