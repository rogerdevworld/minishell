/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:05:47 by rmarrero          #+#    #+#             */
/*   Updated: 2025/04/01 13:05:50 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/minishell.h"

// -- aun hay problemas con las entras y salidas de los comandos -- /
void	ft_here_doc_child(char *delimiter, int *p_fd)
{
	char	*line;

	close(p_fd[0]);
	while (1)
	{
		//write(1, "pipex> ", 7);
		line = get_next_line(STDIN_FILENO);
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
		{
			//ft_printf("sale");
			free(line);
			break ;
		}
		write(p_fd[1], line, ft_strlen(line));
		free(line);
	}
	close(p_fd[1]);
	_exit(0);
}

// -- main function to handle here_doc -- //
void	ft_here_doc(char *delimiter)
{
	int		p_fd[2];
	pid_t	pid;

	if (pipe(p_fd) == -1)
		ft_exit("Pipe creation failed");
	pid = fork();
	if (pid == -1)
		ft_exit("Fork failed");
	if (pid == 0)
		ft_here_doc_child(delimiter, p_fd);
	else
	{
		close(p_fd[1]);
		dup2(p_fd[0], STDIN_FILENO);
		waitpid(pid, NULL, 0);
	}
}
