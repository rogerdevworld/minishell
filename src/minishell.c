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
int	is_heredoc_only(t_ast *ast)
{
	if (!ast || ast->type != NODE_COMMAND)
		return (0);
	if (!ast->cmd)
		return (0);
	if (ast->cmd->args && ast->cmd->args[0])
		return (0);
	if (ast->cmd->redir && ast->cmd->redir->heredoc_count > 0)
		return (1);
	return (0);
}

void	main_loop(t_myenv *myenv)
{
	char		*line;
	t_token		*tokens;
	t_ast		*ast;
	t_minishell	*minishell;
	int			status;

	status = 0;
	while (1)
	{
		line = readline("minishell >");
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
			free_tokens(tokens);
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
		ast = parse_expression(&tokens, myenv->env);
		// /print_ast(ast, 0);
		if (preprocess_heredocs(ast, status) == -1)
		{
			status = 130;
			free_ast(ast);
			free_tokens(tokens);
			free(line);
			continue ;
		}
		else
		{
			minishell = init_minishell(ast, tokens, myenv);
			if (is_heredoc_only(ast))
				status = 0;
			else if (g_signal != S_CANCEL_EXEC)
				status = execute_ast(ast, myenv, minishell, status);
		}
		// Libera memoria
		free_ast(ast);
		free_tokens(tokens);
		free(line);
		g_signal = S_BASE;
	}
}

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