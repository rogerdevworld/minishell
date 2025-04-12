/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:56:13 by rmarrero          #+#    #+#             */
/*   Updated: 2025/03/27 11:57:56 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef EXECUTOR_H
# define EXECUTOR_H

void	ft_check_executor(t_command *cmd, char **envp);
void	ft_exec_cmd(t_command *cmd, char **envp);
void	ft_pipe_and_fork(t_command *cmd, char **env);
void	parent_process(t_command *cmd, int *prev_fd, int p_fd[2]);
void	child_process(t_command *cmd, char **envp, int builtin_id, int prev_fd, int p_fd[2]);
pid_t	external_command(t_command *cmd, char **envp, int builtin_id, int *prev_fd, int p_fd[2]);
void	redirections(t_command *cmd, int *saved_stdin, int *saved_stdout);
void	restore_redirections(int saved_stdin, int saved_stdout);
void print_args(char **args);

#endif