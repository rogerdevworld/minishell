/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   design.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:56:13 by rmarrero          #+#    #+#             */
/*   Updated: 2025/06/29 17:15:15 by xviladri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef DESIGN_H
# define DESIGN_H

// -- lexer.h -- //
char	*path_terminal(void);
char	*get_git_branch(void);
char	*meta_path(char **envp);
char	*get_computer_name(void);
char	*get_user(char **envp);

#endif
