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

static int	verify_sigint(int status)
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
	char		*full_line;
	t_token		*tokens;
	t_ast		*ast;
	t_minishell	*minishell;
	int			status;

	tokens = NULL;
	ast = NULL;
	minishell = NULL;
	status = 0;
	while (1)
	{
		line = readline("minishell >");
		status = verify_sigint(status);
		if (!line)
			break;
		if (*line)
			add_history(line);

		full_line = read_until_balanced(line);
		free(line);
		if (!full_line)
			continue;

		tokens = lexer(full_line);
		if (check_multiple_expansions_at_start(tokens, myenv->list_env))
		{
			status = 1;
			free(full_line);
			free_tokens(tokens);
			continue;
		}

		expand_before_executor(&tokens, myenv->list_env, status);
		shift_empty_tokens(&tokens);

		if (validate_syntax(tokens) || check_unclosed_quotes(full_line))
		{
			status = 2;
			free(full_line);
			free_tokens(tokens);
			continue;
		}

		ast = parse_expression(&tokens, myenv->env);
		if (preprocess_heredocs(ast, status) == -1)
		{
			status = 130;
			free_tokens(tokens);
			free_ast(ast);
			free(full_line);
			continue;
		}
		else
		{
			minishell = init_minishell(ast, tokens, myenv);
			if (is_heredoc_only(ast))
				status = 0;
			else if (g_signal != S_CANCEL_EXEC)
				status = execute_ast(ast, myenv, minishell, status);
		}

		if (minishell)
			free_minishell(minishell);
		if (full_line)
			free(full_line);
		tokens = NULL;
		ast = NULL;
		minishell = NULL;
		g_signal = S_BASE;
	}
	if (tokens)
		free_tokens(tokens);
	if (ast)
		free_ast(ast);
}


int	main(int argc, char **argv, char **env)
{
	t_myenv *myenv;

	(void)argc;
	(void)argv;

	myenv = NULL;
	myenv = ft_myenv(env);
	signal_init();
	shelvl(myenv);
	main_loop(myenv);
	free_myenv(myenv);
	return (0);
}