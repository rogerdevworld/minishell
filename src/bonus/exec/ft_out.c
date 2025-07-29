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
			return (-1); // El error ya fue impreso por expand_redir_wildcard
		// NOTA: Esta lógica asume que la redirección es de tipo TRUNCATE ('>').
		// Para soportar APPEND ('>>'), necesitarías guardar el tipo de redirección
		// durante el parseo.
		fd = ft_open(expanded_file, 1);
		if (fd == -1)
		{
			perror(expanded_file);
			free(expanded_file);
			return (-1);
		}
		free(expanded_file);
		if (redir->output_file > 2)
			close(redir->output_file);
		redir->output_file = fd;
		i++;
	}
	return (0);
}