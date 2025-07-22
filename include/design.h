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
char	*get_user(char **envp);

char	*ft_desing(char **env, int status);
char	*join_dir_with_slash(char *base, char *dir, int add_slash);
char	*build_short_path(char **dirs);
char	*build_long_path(char **dirs, int len);
char	*build_colored_prompt_part(char *user, char *path, int status);

#endif