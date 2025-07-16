/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xviladri <xviladri@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:43:57 by xviladri          #+#    #+#             */
/*   Updated: 2025/04/22 20:24:20 by xviladri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

void	init_redir(t_redir *redir)
{
	if (!redir)
		return ;
	redir->input_file = -2;
	redir->output_file = -2;
	redir->limiter = NULL;
	redir->in_file = ft_calloc(64, sizeof(char *));
	redir->out_file = ft_calloc(64, sizeof(char *));
}

void	free_redir(t_redir *redir)
{
	if (!redir)
		return ;
	if (redir->limiter)
		free(redir->limiter);
	if (redir->in_file)
		free(redir->in_file);
	if (redir->out_file)
		free(redir->out_file);
	free(redir);
}
void	reset_redir(t_redir *redir)
{
	if (!redir)
		return ;
	if (redir->limiter)
		free(redir->limiter);
	if (redir->in_file)
		free(redir->in_file);
	if (redir->out_file)
		free(redir->out_file);
	init_redir(redir);
}
