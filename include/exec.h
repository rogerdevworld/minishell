/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:56:13 by rmarrero          #+#    #+#             */
/*   Updated: 2025/04/09 22:22:59 by xviladri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

// --- Forward declarations to break circular dependencies ---
struct	s_ast;
struct	s_myenv;
struct	s_minishell;
struct	s_redir;
struct	s_command;

// -- Main Executor --
int		execute_ast(struct s_ast *node, struct s_myenv *myenv,
			struct s_minishell *minishell, int status);

// -- Command Types --
int		execute_pipe(struct s_ast *node, struct s_myenv *myenv,
			struct s_minishell *minishell);
int		execute_subshell(struct s_ast *node, struct s_myenv *myenv,
			struct s_minishell *minishell);
int		execute_and(struct s_ast *node, struct s_myenv *myenv,
			struct s_minishell *minishell);
int		execute_or(struct s_ast *node, struct s_myenv *myenv,
			struct s_minishell *minishell);
int		execute_bg(struct s_ast *node, struct s_myenv *myenv,
			struct s_minishell *minishell);
int		execute_command(struct s_command *cmd,
			struct s_minishell *minishell, int status);

// -- Heredoc Functions --
int		preprocess_heredocs(struct s_ast *node, int status);
int		expand_heredoc_limiter(struct s_redir *redir);
char	*expand_line_heredoc(const char *line, int status);
char	*get_var_name(const char **p);
int		handle_fork_error(int *pipefd);
int		handle_child_exit(int status, int *pipefd);

// -- Redirection Functions --
int		ft_output_redirections(struct s_redir *redir);
int		ft_input_redirection(struct s_redir *redir);

#endif