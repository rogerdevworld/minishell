/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:41:56 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:42:01 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef EXEC_H
# define EXEC_H

// -- exec main all cases -- //
int	execute_ast(t_ast *node, t_myenv *myenv, t_minishell *minishell,
		int status);

// -- ft_pipe -- //
int	execute_pipe(t_ast *node, t_myenv *myenv, t_minishell *minishell);

// -- here doc's -- //
int	preprocess_heredocs(t_ast *node, int status);
int	process_all_heredocs(t_redir *redir, int status);
int	process_single_heredoc(char *limiter, int *fd_out, int status);

// -- ft_subshell -- //
int	execute_subshell(t_ast *node, t_myenv *myenv, t_minishell *minishell);

int							execute_bg(t_ast *node, t_myenv *myenv, t_minishell *minishell);

// -- ft_and & ft_or -- //
int	execute_and(t_ast *node, t_myenv *myenv, t_minishell *minishell);
int	execute_or(t_ast *node, t_myenv *myenv, t_minishell *minishell);

// -- ft_in & ft_out -- //
int	ft_output_redirections(t_redir *redir);
int	ft_input_redirection(t_redir *redir);

// -- ft_cmd normal commands + builtings -- //
// int	execute_command(t_command *cmd, char **envp, t_myenv *myenv,
// 		t_minishell *minishell, int status);
int	execute_command(t_command *cmd, t_minishell *minishell, int status);

#endif
