/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 15:46:22 by rmarrero          #+#    #+#             */
/*   Updated: 2024/09/13 12:59:38 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*copy;
	size_t	len;

	len = ft_strlen(s) + 1;
	copy = malloc(len);
	if (!copy)
		return (NULL);
	ft_memcpy(copy, s, len);
	return (copy);
}
