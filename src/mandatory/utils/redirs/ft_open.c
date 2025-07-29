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

// src/utils/redirs/ft_open.c

int	ft_open(char *file, int mode)
{
	int		fd;
	char	*clean_file; // Variable temporal

	clean_file = remove_quotes(file); // Guardamos la memoria nueva aquí
	if (!clean_file)
		return (-1); // Manejo de error si malloc falla

	if (mode == 0)
		fd = open(clean_file, O_RDONLY);
	if (mode == 1)
		fd = open(clean_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (mode == 2)
		fd = open(clean_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	
	free(clean_file); // Liberamos la memoria que ya no necesitamos
	
	return (fd);
}