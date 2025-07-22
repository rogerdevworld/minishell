/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 20:16:29 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/16 20:16:34 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

int	ft_input_redirection(t_redir *redir)
{
	int		fd;
	char	**files;
	int		i;

	if (!redir->in_file)
		return (0);
	files = redir->in_file;
	i = 0;
	while (files[i])
	{
		fd = ft_open(files[i], 0);
		if (fd == -1)
		{
			perror(files[i]);
			return (-1);
		}
		if (redir->input_file != -1)
			close(redir->input_file);
		redir->input_file = fd;
		i++;
	}
	return (0);
}
