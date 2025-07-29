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
#include "../../../include/minishell.h"

// src/exec/ft_in.c

int	ft_input_redirection(t_redir *redir)
{
	int		fd;
	char	**files;
	int		i;
	char	*expanded_file;

	if (!redir || !redir->in_file)
		return (0);
	files = redir->in_file;
	i = 0;
	while (files[i])
	{
		expanded_file = expand_redir_wildcard(files[i]);
		if (!expanded_file)
			return (-1); // El error ya fue impreso por expand_redir_wildcard
		fd = ft_open(expanded_file, 0);
		if (fd == -1)
		{
			perror(expanded_file);
			free(expanded_file);
			return (-1);
		}
		free(expanded_file);
		if (redir->input_file > 2)
			close(redir->input_file);
		redir->input_file = fd;
		i++;
	}
	return (0);
}