/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:36:52 by rmarrero          #+#    #+#             */
/*   Updated: 2025/03/27 17:36:53 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../include/minishell.h"

void ft_destroyer(t_minishell *minishell)
{
    free_minishell(minishell);
    ft_myenv_free(minishell->env);
}