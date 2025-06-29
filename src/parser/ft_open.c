/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_open.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:05:47 by rmarrero          #+#    #+#             */
/*   Updated: 2025/06/28 17:56:40 by xviladri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

int	ft_open(char *file, int mode)
{
	int	fd;

	if (mode == 0)
		fd = open(file, O_RDONLY);
	if (mode == 1)
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (mode == 2)
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (fd == -1)
		ft_exit("Failed to open file");
	return (fd);
}

t_operator	resolve_operator(char *operator)
{
	if (ft_strncmp(operator, "||", 2) == 0)
		return (OR);
	if (ft_strcmp(operator, "&&") == 0)
		return (AND);
	if (ft_strcmp(operator, "|") == 0)
		return (PIPE);
	return (COMMAND);
}

const char	*operator_to_str(t_operator op)
{
	if (op == PIPE)
		return ("|");
	else if (op == AND)
		return ("&&");
	else if (op == OR)
		return ("||");
	else if (op == COMMAND)
		return ("COMMAND");
	else
		return ("UNKNOWN");
}
