/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:56:13 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/03 14:25:59 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef EXECUTOR_H
# define EXECUTOR_H

typedef struct s_executor
{
	int						status;
	int						p_fd[2];
	int						prev_fd;
	pid_t					pid;
	int						builtin_id;
	int						saved_stdin;
	int						saved_stdout;
	t_myenv					*myenv;
	char					**envp;
}							t_executor;

typedef struct s_minishell	t_minishell;

/**
 * aqui montare las nuevas funciones para
 * el nuevo executor
 */
// // int							executor_loop(t_minishell *minishell,
// 								int status);

/**
 * funciones viejas
 */

// executor.h o en minishell.h
t_executor					*init_exec(t_myenv *myenv);

// // int							ft_check_executor(t_minishell *minishell,
// // 								t_executor *exec, t_command *cmd, char **env,
// // 								t_myenv *myenv, int s);
// // void						ft_exec_cmd(t_command *cmd, char **envp);
// // void						ft_pipe_and_fork(t_command *cmd, char **env);
// // pid_t						external_command(t_command *cmd,
// 							t_executor *ex);
// // // void		child_process(t_command *cmd, t_executor *ex);
// // // void		parent_process(t_command *cmd, t_executor *ex);
// // void						redirections(t_command *cmd, int *saved_stdin,
// // 								int *saved_stdout);
// // void						restore_redirections(int saved_stdin,
// // 								int saved_stdout);
// // void						print_args(char **args);
// // // void		ft_check_executor_single(t_minishell *minishell,
// 			t_executor *exec,
// // // t_command *cmd, char **envp, t_myenv *myenv);
// // // int			execute_command_list(t_minishell *minishell,
// 				t_executor *exec,
// // // t_command *cmd, char **envp, t_myenv *myenv);
// // void						ft_check_executor_single(t_minishell *minishell,
// // 								t_command *cmd);
// // int							execute_command_list(t_minishell *minishell);
// // int							execute_astint(t_minishell *minishell,
// // 								t_executor *exec, t_command *cmd, char **envp,
// // 								t_myenv *myenv);
// // int							execute_ast(t_ast *node, char **envp,
// // 								t_myenv *myenv, t_minishell *minishell,
// // 								int status);

// -- exec main all cases -- //
int							execute_ast(t_ast *node, char **envp,
								t_myenv *myenv, t_minishell *minishell,
								int status);

// -- ft_pipe -- //
int	execute_pipe(t_ast *node, char **envp, t_myenv *myenv,
	t_minishell *minishell);

// -- here doc's -- //
int							process_all_heredocs(t_redir *redir);
int							process_single_heredoc(char *limiter, int *fd_out);

// -- ft_subshell -- //
int							execute_subshell(t_ast *node, char **envp,
								t_myenv *myenv, t_minishell *minishell);

// -- ft_and & ft_or -- //
int							execute_and(t_ast *node, char **envp,
								t_myenv *myenv, t_minishell *minishell);
int							execute_or(t_ast *node, char **envp, t_myenv *myenv,
								t_minishell *minishell);

// -- ft_in & ft_out -- //
int							ft_output_redirections(t_redir *redir);
int							ft_input_redirection(t_redir *redir);

// -- ft_cmd normal commands + builtings -- //
int							execute_command(t_command *cmd, char **envp,
								t_myenv *myenv, t_minishell *minishell,
								int status);
#endif
