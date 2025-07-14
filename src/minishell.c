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
 * nueva version de main_loop
 */
void	main_loop(char **env)
{
	char		*line;
	t_token		*tokens;
	t_command	*cmd;
	t_ast		*ast;
	t_executor	*exec;
	t_myenv		*myenv;
	t_minishell	*minishell;
	int			status;

	status = 0;
	tokens = NULL;
	myenv = ft_myenv(env);
	while (1)
	{
		// line = readline(ft_agnoster(envp, status));
		// line = readline(ft_agnoster(envp, status));
		// line = readline(ft_strjoin("mini > ", ft_itoa(status)));
		line = readline("mini > ");
		status = verify_sigint(status);
		if (!line)
			break ;
		if (*line)
			add_history(line);
		tokens = lexer(line);
		if (validate_syntax(tokens) || check_unclosed_quotes(line))
			status = 2;
		else
		{
			// print_tokens(tokens);
			ast = parse_expression(&tokens, env);
			// print_ast(ast, 0);
			exec = init_exec(myenv);
			minishell = init_minishell(ast, tokens, cmd, exec);
			if (g_signal != S_CANCEL_EXEC)
				status = execute_ast(ast, env, myenv, minishell, status);
		}
		free(line);
	}
	g_signal = S_BASE;
}

int	main(int argc, char **argv, char **env)
{
	(void)argc;
	(void)argv;

	signal_init();
	main_loop(env);
	return (0);
}