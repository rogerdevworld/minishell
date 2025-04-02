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

// -- main loop for minishell -- //
void	main_loop(int *p_fd)
{
	char	*line;
	t_token	*token;

	token = NULL;
	close(p_fd[0]);
	while (1)
	{
		line = readline("minishell> ");
		if (!line)
			break ;
		token = lexer(line);
		if (*line)
			add_history(line);
		token = lexer(line);
		// -- lo deberia hacer el execve(); clasificar si es local o de sistema -- //
		while (token)
		{
			if (get_builtin_cmd(token->value))
				execute_builtin(get_builtin_cmd(token->value), token);
			token = token->next;
		}
		if (ft_strncmp(line, "exit", 4) == 0)
		{
			free(line);
			break ;
		}
		print_tokens(token);
		free(line);
	}
	close(p_fd[1]);
	exit(0);
}

// -- main function to handle here_doc -- //
void	ft_here_doc(void)
{
	int		p_fd[2];
	pid_t	pid;

	if (pipe(p_fd) == -1)
		ft_exit(1, "Pipe creation failed");
	pid = fork();
	if (pid == -1)
		ft_exit(1, "Fork failed");
	if (pid == 0)
		main_loop(p_fd);
	else
	{
		close(p_fd[1]);
		dup2(p_fd[0], STDIN_FILENO);
		waitpid(pid, NULL, 0);
	}
}

int	main(void)
{
	ft_here_doc();
	return (0);
}

// -- commad for test -- //
// ls -la || cat -e && echo "holo world" > new_text || text; echo hola
//	&& $VAR = range
