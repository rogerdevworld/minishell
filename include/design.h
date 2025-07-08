/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   desing.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:56:13 by rmarrero          #+#    #+#             */
/*   Updated: 2025/03/27 11:57:56 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef DESING_H
# define DESING_H

// -- lexer.h -- //
char	*path_terminal(void);
char	*get_git_branch(void);
char	*ft_agnoster(char **envp, int status);
char	*get_computer_name(void);
char	*get_user(char **envp);

char	*ft_desing(char **envp);
#endif