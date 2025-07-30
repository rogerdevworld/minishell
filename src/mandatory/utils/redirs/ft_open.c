/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_open.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:05:47 by rmarrero          #+#    #+#             */
/*   Updated: 2025/04/22 12:45:57 by xviladri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../../include/minishell.h"

/**
 * Opens a file with specified mode (read-only, write/create/truncate, 
 	or write/create/append).
 * It first removes any quotes from the filename to ensure correct 
 	path resolution.
 * Returns the file descriptor on success, or -1 on failure (and sets errno).
 */
int	ft_open(char *file, int mode)
{
	int		fd;
	char	*clean_file;

	clean_file = remove_quotes(file);
	if (!clean_file)
		return (-1);
	if (mode == 0)
		fd = open(clean_file, O_RDONLY);
	if (mode == 1)
		fd = open(clean_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (mode == 2)
		fd = open(clean_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	free(clean_file);
	return (fd);
}
