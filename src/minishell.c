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

// -- exit -- //
void	ft_exit(int status, char *msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putendl_fd(msg, STDERR_FILENO);
	exit(status);
}

// -- main loop for minishell -- //
void	main_loop(char *delimiter, int *p_fd)
{
	char	*line;
	t_token	*tokens;

	tokens = NULL;
	close(p_fd[0]);
	while (1)
	{
		write(1, "minishell> ", 11);
		line = get_next_line(STDIN_FILENO);
		tokens = lexer(line);
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
		{
			free(line);
			break ;
		}
		print_tokens(tokens);
		free(line);
	}
	close(p_fd[1]);
	exit(0);
}

// -- main function to handle here_doc -- //
void	ft_here_doc(char *delimiter)
{
	int		p_fd[2];
	pid_t	pid;

	if (pipe(p_fd) == -1)
		ft_exit(1, "Pipe creation failed");
	pid = fork();
	if (pid == -1)
		ft_exit(1, "Fork failed");
	if (pid == 0)
		main_loop(delimiter, p_fd);
	else
	{
		close(p_fd[1]);
		dup2(p_fd[0], STDIN_FILENO);
		waitpid(pid, NULL, 0);
	}
}

void	print_tokens(t_token *tokens)
{
	while (tokens)
	{
		ft_putstr_fd("Token Type -> ", 1);
		ft_putstr_fd(token_type_to_string(tokens->type), 1);
		ft_putstr_fd("-> ", 2);
		ft_putstr_fd(tokens->value, 1);
		ft_putstr_fd("\n", 1);
		tokens = tokens->next;
	}
}

int	main(void)
{
	ft_here_doc("hola");
	return (0);
}

// -- commad for test -- //
// ls -la || cat -e && echo "holo world" > new_text || text; echo hola && $VAR = range
