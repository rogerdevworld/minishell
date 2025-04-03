/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 21:49:28 by rmarrero          #+#    #+#             */
/*   Updated: 2025/02/22 21:49:33 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/minishell.h"
#include "../include/parser.h"

// -- main loop for minishell -- //
void	main_loop(char **envp)
{
	char		*line;
	t_token		*tokens;
	t_command	*cmd;

	tokens = NULL;
	envp = envp;
	while (1)
	{
		line = readline(meta_path(envp));
		if (!line)
			break ;
		tokens = lexer(line);
		if (*line)
			add_history(line);
		if (ft_strncmp(line, "exit", 4) == 0)
		{
			free(line);
			break ;
		}
		cmd = parse_tokens(tokens, envp);
		ft_check_executor(cmd, envp);
		free(line);
	}
	exit(0);
}

int	main(int argc, char **argv, char **envp)
{
	argc = argc;
	argv = argv;
	main_loop(envp);
	return (0);
}

// -- commad for test -- //
/*
ls -la > text && cat -e text && echo "holo world" > new_text && cat text
&& echo hola && cat < new_text || ls -la > text && wc - l text > text_c
&& cat text*
*/

// valgrind --leak-check=full --show-leak-kinds=all ./minishell
// valgrind --leak-check=full ./minishell