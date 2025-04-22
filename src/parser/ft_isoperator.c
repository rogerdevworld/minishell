/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isoperator.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xviladri <xviladri@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:43:57 by xviladri          #+#    #+#             */
/*   Updated: 2025/04/22 20:27:23 by xviladri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

int	ft_isoperator(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (-1);
	if (ft_strcmp(str, "&&" ) == 0)
		return (1);
	else if (ft_strcmp(str, "||" ) == 0)
		return (2);
	return (0);
}
