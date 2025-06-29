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
}			t_executor;

// executor.h o en minishell.h

void		execute_command(t_command *cmd, t_executor *ex, pid_t *pids,
				int *i);
void		ft_check_executor(t_command *cmd, char **envp, t_myenv *myenv);
int			handle_builtin(t_command *cmd, t_executor *ex, char **envp,
				t_myenv *myenv);
void		execute_builtin_command(t_command *cmd, t_executor *ex, char **envp,
				t_myenv *myenv);
pid_t		external_command(t_command *cmd, t_executor *ex);
void		child_process(t_command *cmd, t_executor *ex, int pipefd[2]);
void		execute_external_command(t_command *cmd, t_executor *ex);
void		ft_exec_cmd(t_command *cmd, char **envp);
void		ft_pipe_and_fork(t_command *cmd, char **env);
void		parent_process(t_command *cmd, t_executor *ex);
void		print_args(char **args);

#endif
