/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:56:13 by rmarrero          #+#    #+#             */
/*   Updated: 2025/03/27 11:57:56 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef UTILS_H
# define UTILS_H

void	ft_shlvl(t_minishell *minishell);
char	*get_path(char *cmd, char **env);
char	**ft_split_quotes(char const *s, char c);
int	ft_open(char *file, int mode);


// -- free -- // 
void	free_split(char **split);
void	free_minishell(t_minishell *minishell);

// -- test -- // 
void	print_tokens(t_token *tokens);

#endif