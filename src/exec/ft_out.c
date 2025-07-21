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

int	ft_output_redirections(t_redir *redir)
{
	int		i;
	int		fd;
	char	**files;

	i = 0;
	files = redir->out_file;
	while (files && files[i])
	{
		if (redir->output_file != -1)
			close(redir->output_file);
		fd = ft_open(files[i], 1);
		if (fd == -1)
		{
			perror("open");
			return (-1);
		}
		redir->output_file = fd;
		i++;
	}
	return (0);
}
