/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:56:13 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/03 14:26:42 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef REDIRECTIONS_H
# define REDIRECTIONS_H

// -- redirections.h -- //
int		ft_open(char *file, int mode);

// -- here doc's -- //
int		ft_here_doc(char *delimiter);
void	ft_here_doc_child(char *delimiter, int *p_fd);

#endif
