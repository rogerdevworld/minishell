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
// line = readline(ft_agnoster(envp, status));
// line = readline(ft_agnoster(envp, status));
// line = readline(ft_strjoin("mini > ", ft_itoa(status)));
void	main_loop(t_myenv *myenv)
{
	char		*line;
	t_token		*tokens;
	t_ast		*ast;
	t_executor	*exec;
	t_minishell	*minishell;
	int			status = 0;
	char *prompt = ft_desing(myenv->env, status);

	tokens = NULL;
	while (1)
	{
		line = readline(prompt);
		free(prompt);
		//line = readline("mini>");
		status = verify_sigint(status);
		if (!line)
			break ;
		if (*line)
			add_history(line);
		tokens = lexer(line);
		//print_tokens(tokens);
		ast = parse_expression(&tokens, myenv->env);
		if (check_multiple_expansions_at_start(tokens, myenv->list_env))
		{
			status = 1;
			continue ;
		}
		expand_before_executor(&tokens, myenv->list_env, status);
		shift_empty_tokens(&tokens);
		if (validate_syntax(tokens) || check_unclosed_quotes(line))
			status = 2;
		else if (ft_strcmp(line, "./minishell") == 0)
		{
			ft_shlvl(myenv);
			clear_history();
		}
		// else if (!ast->cmd->args[0] && (ast->cmd->redir->in_file || ast->cmd->redir->out_file))
		// {
		// 	i = 0;
		// 	while (ast->cmd->redir->in_file && ast->cmd->redir->in_file[i])
		// 	{
		// 		if (open(ast->cmd->redir->in_file[i], O_RDONLY) == -1)
		// 		{
		// 			perror(ast->cmd->redir->in_file[i]);
		// 			status = 1;
		// 			break ;
		// 		}
		// 		i++;
		// 	}
		// 	i = 0;
		// 	while (ast->cmd->redir->out_file && ast->cmd->redir->out_file[i])
		// 	{
		// 		if (open(ast->cmd->redir->out_file[i], O_CREAT | O_WRONLY | O_TRUNC, 0644) == -1)
		// 		{
		// 			perror(ast->cmd->redir->out_file[i]);
		// 			status = 1;
		// 			break ;
		// 		}
		// 		i++;
		// 	}
		// }
		else
		{
			// print_tokens(tokens);
			//print_ast(ast, 0);
			exec = init_exec(myenv);
			minishell = init_minishell(ast, tokens, exec);
			if (g_signal != S_CANCEL_EXEC)
				status = execute_ast(ast, myenv->env, myenv, minishell, status);
		}
		// ft_destroyer(minishell);
		free(line);
	}
	g_signal = S_BASE;
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