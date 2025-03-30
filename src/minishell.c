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


void	ft_exit(int status, char *msg)
{
	ft_putstr_fd("pipex: ", STDERR_FILENO);
	ft_putendl_fd(msg, STDERR_FILENO);
	exit(status);
}

void	ft_here_doc_child(char *delimiter, int *p_fd)
{
	char	*line;

	close(p_fd[0]);
	while (1)
	{
		write(1, "minishell> ", 11);
		line = get_next_line(STDIN_FILENO);
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
		{
			free(line);
			break ;
		}
		write(p_fd[1], line, ft_strlen(line));
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
		ft_here_doc_child(delimiter, p_fd);
	else
	{
		close(p_fd[1]);
		dup2(p_fd[0], STDIN_FILENO);
		waitpid(pid, NULL, 0);
	}
}

int	main(void)
{
	ft_here_doc("hola");
	//lexer(argv[1]);
	return (0);
}
