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

// -- parser and ft_export -- //
char	*remove_quotes(char *str);
char	*get_path(char *cmd, char **env);

// -- lexer -- //
char	**ft_split_quotes(char const *s, char c);

// -- builtings -- //
void	ft_shlvl(t_minishell *minishell);


// -- utils -- // 
int		ft_open(char *file, int mode);

// -- free -- //
void	free_split(char **split);
void	free_minishell(t_minishell *minishell);

// -- test -- //
void	print_tokens(t_token *tokens);
void	print_command(t_command *cmd);
void	print_ast(t_ast *node, int depth);

#endif