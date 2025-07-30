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

/**
 * Handles output redirections for a command.
 * It iterates through the output files, expands any wildcards in their names,
 * opens each file (creating or truncating for '>' or appending for '>>'),
 * and sets the last opened file as the standard output for the command.
 * Closes previously opened output files to ensure only the
	last one is effective.
 * Returns 0 on success, or -1 on error (e.g., permission denied,
	or wildcard expansion failure).
 */
int	ft_output_redirections(t_redir *redir)
{
	int		fd;
	char	**files;
	int		i;
	char	*expanded_file;

	if (!redir || !redir->out_file)
		return (0);
	files = redir->out_file;
	i = 0;
	while (files[i])
	{
		expanded_file = expand_redir_wildcard(files[i]);
		if (!expanded_file)
			return (-1);
		fd = ft_open(expanded_file, 1);
		if (fd == -1)
			return (perror(expanded_file), free(expanded_file), -1);
		free(expanded_file);
		if (redir->output_file > 2)
			close(redir->output_file);
		redir->output_file = fd;
		i++;
	}
	return (0);
}
