/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:56:13 by rmarrero          #+#    #+#             */
/*   Updated: 2025/06/29 17:17:11 by xviladri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef EXECUTOR_H
# define EXECUTOR_H

typedef struct s_executor
{
	int		p_fd[2];
	int		prev_fd;
	pid_t	pid;
	int		builtin_id;
	int		saved_stdin;
	int		saved_stdout;
	t_myenv	*myenv;
	char	**envp;
}	t_executor;

// executor.h o en minishell.h

void	ft_check_executor(t_command *cmd, char **envp, t_myenv *myenv);
void	ft_exec_cmd(t_command *cmd, char **envp);
void	ft_pipe_and_fork(t_command *cmd, char **env);
pid_t	external_command(t_command *cmd, t_executor *ex);
void	child_process(t_command *cmd, t_executor *ex);
void	parent_process(t_command *cmd, t_executor *ex);
void	redirections(t_command *cmd, int *saved_stdin, int *saved_stdout);
void	restore_redirections(int saved_stdin, int saved_stdout);
void	print_args(char **args);

#endif
