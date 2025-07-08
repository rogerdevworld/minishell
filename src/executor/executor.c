/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:05:47 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/03 14:43:09 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

/**
 * executor actualizado de GPT probando
 */
int	ft_check_executor(t_minishell *minishell, t_executor *exec, t_command *cmd,
	char **envp, t_myenv *myenv)
{
	int		pipefd[2];
	int		i = 0;
	pid_t	pids[1024];
	t_command *current = cmd;

	if (!exec)
		exec = init_exec(myenv);
	if (!exec)
		return (1);

	while (current)
	{
		if (current->operator == 2)
		{
			//ft_printf("ha entrado");
			//ft_printf("\nel estatus es: \n%i", minishell->exit);
			if (minishell->exit != 0) // Falló el anterior
			{
				current = current->next;
				if (current) current = current->next;
				continue ;
			}
			current = current->next;
			continue ;
		}
		//else if (ft_strcmp(current->args[0], "||") == 0)
		else if (current->operator == 3)
		{
			if (minishell->exit == 0) // Éxito previo, no ejecutar lo siguiente
			{
				current = current->next;
				if (current) current = current->next;
				continue ;
			}
			current = current->next;
			continue ;
		}
		exec->builtin_id = get_builtin_cmd(current->args[0]);

		//ft_printf("\nel operador acutal es  %i\n", current->operator );
		//ft_printf("\nel current args es: %s\n", current->args[0]);
		// Manejo de operadores lógicos
		//if (ft_strcmp(current->args[0], "&&") == 0)
		ft_printf("status executor antes de : %i\n", minishell->exit);
		if (exec->builtin_id != -1 && current->operator != PIPE && exec->prev_fd == -1)
		{
			int saved_stdin = dup(STDIN_FILENO);
			int saved_stdout = dup(STDOUT_FILENO);

			if (current->input_file != -1)
			{
				dup2(current->input_file, STDIN_FILENO);
				close(current->input_file);
			}
			if (current->output_file != -1)
			{
				dup2(current->output_file, STDOUT_FILENO);
				close(current->output_file);
			}

			minishell->exit = execute_builtin(minishell, current->args, envp, myenv);

			dup2(saved_stdin, STDIN_FILENO);
			dup2(saved_stdout, STDOUT_FILENO);
			close(saved_stdin);
			close(saved_stdout);

			current = current->next;
			continue;
		}

		// PIPE
		if (current->operator == PIPE)
			if (pipe(pipefd) == -1)
				ft_exit("pipe failed");

		g_signal = S_CMD;
		exec->pid = fork();
		if (exec->pid == -1)
			ft_exit("fork failed");

		if (exec->pid == 0) // CHILD
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);

			if (current->input_file != -1)
				dup2(current->input_file, STDIN_FILENO);
			else if (exec->prev_fd != -1)
				dup2(exec->prev_fd, STDIN_FILENO);

			if (current->output_file != -1)
				dup2(current->output_file, STDOUT_FILENO);
			else if (current->operator == PIPE)
			{
				close(pipefd[0]);
				dup2(pipefd[1], STDOUT_FILENO);
			}

			if (exec->prev_fd != -1)
				close(exec->prev_fd);

			if (current->operator == PIPE)
				close(pipefd[0]);

			if (exec->builtin_id != -1)
				exit(execute_builtin(exec->builtin_id, current->args, envp, myenv));

			char *path = NULL;
			if (current->args[0][0] == '/' || current->args[0][0] == '.')
				path = ft_strdup(current->args[0]);
			else
				path = get_path(current->args[0], envp);

			if (!path || access(path, X_OK) != 0)
			{
				write(2, "minishell: command not found\n", 30);
				if (path) free(path);
				exit(127);
			}
			execve(path, current->args, myenv->env);
			perror("execve");
			exit(127);
		}
		else // PARENT
		{
			pids[i++] = exec->pid;
			if (exec->prev_fd != -1)
				close(exec->prev_fd);
			if (current->operator == PIPE)
			{
				close(pipefd[1]);
				exec->prev_fd = pipefd[0];
			}
			else
				exec->prev_fd = -1;
		}
		current = current->next;
	}

	// Espera a todos los hijos y guarda solo el status del último
	for (int j = 0; j < i - 1; j++)
		waitpid(pids[j], NULL, 0);
	if (i > 0)
		waitpid(pids[i - 1], &(exec->status), 0);

	// Actualiza minishell->exit según el resultado
	if (WIFEXITED(exec->status))
		minishell->exit = WEXITSTATUS(exec->status);
	else if (WIFSIGNALED(exec->status))
	{
		int signo = WTERMSIG(exec->status);
		if (signo == SIGPIPE)
			minishell->exit = 0;
		else if (signo == SIGINT)
		{
			minishell->exit = 130;
			write(1, "\n", 1);
		}
		else if (signo == SIGQUIT)
		{
			minishell->exit = 131;
			write(1, "Quit (core dumped)\n", 19);
		}
		else
			minishell->exit = 128 + signo;
	}

	if (g_signal == S_SIGINT_CMD)
		minishell->exit = 130;

	g_signal = S_BASE;
	ft_printf("status executor: %i\n", minishell->exit);
	return minishell->exit;
}


/**
 * este executor funciona con el build_ast antiguo
 */
/* int	ft_check_executor(t_minishell *minishell, t_executor *exec, t_command *cmd,
		char **envp, t_myenv *myenv)
{
	int		pipefd[2];
	int		i;
	int		saved_stdin;
	int		saved_stdout;
	char	*path;
	int		last_builtin_result;
	int		signo;

	// char	*path;
	i = 0;
	pid_t pids[1024]; // Máximo número de comandos
	last_builtin_result = 0;
	// Inicializar la estructura exec si no está inicializada
	if (!exec)
		exec = init_exec(myenv);
	if (!exec)
		return (1);
	// ft_printf("entra");
	cmd = minishell->cmd;
	while (cmd)
	{
		// if (cmd->limiter)
		// 	ft_here_doc(cmd->limiter);
		// if (!cmd->args || !cmd->args[0])
		// {
		// 	//write(2, "DEBUG: command sin args\n", 24);
		// 	//if (cmd->limiter)
		// 	//{
		// 	//	write(2, "DEBUG: heredoc en un command sin args\n", 38);
		// 	//	int tmp_fd = ft_here_doc(cmd->limiter);
		// 	//	close(tmp_fd);
		// 	//}
		// 	//cmd = cmd->next;
		// 	//continue ;
		// 	cmd = cmd->next;
		// 	continue ;
		// }
		if (ft_strcmp(cmd->args[0], "&&") == 0)
		//if (cmd->args && cmd->args[0] && ft_strcmp(cmd->args[0], "&&") == 0)
		{
			printf("minishell exit status: %i\n", minishell->exit);
			//if (minishell->exit != 0)
			if (minishell->exit != 0)
			{
				//printf("minishell exit status: %i\n", minishell->exit);
				cmd = cmd->next;
			}
			cmd = cmd->next;
			continue;
		}
		//else if (cmd->args && cmd->args[0] && ft_strcmp(cmd->args[0], "||") == 0)
		else if (ft_strcmp(cmd->args[0], "||") == 0)
		{
			if (minishell->exit == 0)
				cmd = cmd->next;
			cmd = cmd->next;
			continue;
		}
		exec->builtin_id = get_builtin_cmd(cmd->args[0]);
		
		// Ejecutar built-in directamente si no hay pipe
		if (exec->builtin_id != -1 && cmd->operator!= PIPE && exec->prev_fd ==
			- 1)
		{
			saved_stdin = dup(STDIN_FILENO);
			saved_stdout = dup(STDOUT_FILENO);
			if (cmd->input_file != -1)
			{
				dup2(cmd->input_file, STDIN_FILENO);
				close(cmd->input_file);
			}
			if (cmd->output_file != -1)
			{
				dup2(cmd->output_file, STDOUT_FILENO);
				close(cmd->output_file);
			}
			last_builtin_result = execute_builtin(exec->builtin_id, cmd->args,
					envp, myenv);
			ft_printf("\nlast_builtin_result es: %i \n", last_builtin_result);
			dup2(saved_stdin, STDIN_FILENO);
			dup2(saved_stdout, STDOUT_FILENO);
			close(saved_stdin);
			close(saved_stdout);
			cmd = cmd->next;
			continue ;
		}
		if (cmd->operator== PIPE)
		{
			if (pipe(pipefd) == -1)
				ft_exit("pipe failed");
		}
		g_signal = S_CMD;
		exec->pid = fork();
		if (exec->pid == -1)
			ft_exit("fork failed");
		if (exec->pid == 0) // Child
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			if (cmd->input_file != -1)
			{
				dup2(cmd->input_file, STDIN_FILENO);
				close(cmd->input_file);
			}
			else if (exec->prev_fd != -1)
			{
				dup2(exec->prev_fd, STDIN_FILENO);
				close(exec->prev_fd);
			}
			if (cmd->output_file != -1)
			{
				dup2(cmd->output_file, STDOUT_FILENO);
				close(cmd->output_file);
			}
			else if (cmd->operator== PIPE)
			{
				close(pipefd[0]);
				dup2(pipefd[1], STDOUT_FILENO);
				close(pipefd[1]);
			}
			if (exec->prev_fd != -1)
				close(exec->prev_fd);
			// if (minishell->ast_tree->op == AND && minishell->exit == 0)
			//{
			//	ft_printf("segundo comando");
			//}
			if (cmd->operator== PIPE)
				close(pipefd[0]);
			if (exec->builtin_id != -1)
			{
				execute_builtin(exec->builtin_id, cmd->args, envp, myenv);
				ft_printf("paso\n");
				exit(exec->status);
			}
			else
			{
				path = NULL;
				if (cmd->args[0][0] == '/' || cmd->args[0][0] == '.')
					path = ft_strdup(cmd->args[0]);
				else
					path = get_path(cmd->args[0], envp);
				if (!path || access(path, X_OK) != 0)
				{
					// ft_printf("minishell: %s: command not found\n",
					//	cmd->args[0]);
					write(2, "minishell: command not found\n", 30);
					if (path)
						free(path);
					exit(127);
				}
				execve(path, cmd->args, myenv->env);
				perror("execve");
				exit(127);
			}
		}
		else // Parent
		{
			pids[i++] = exec->pid;
			if (exec->prev_fd != -1)
				close(exec->prev_fd);
			if (cmd->operator== PIPE)
			{
				close(pipefd[1]);
				exec->prev_fd = pipefd[0];
			}
			else
				exec->prev_fd = -1;
		}
		cmd = cmd->next;
	}
	for (int j = 0; j < i - 1; j++)
		waitpid(pids[j], NULL, 0); // esperar sin guardar estado

	if (i > 0)
		waitpid(pids[i - 1], &(exec->status), 0); // guardar el status del último

	// *
	// 	* cambio para integrar senales
		
	if (WIFEXITED(exec->status))
	{
		// Actualiza el minishell->exit_status
		if (minishell)
			minishell->exit = WEXITSTATUS(exec->status);
		else
			minishell->exit = WEXITSTATUS(exec->status);
	}
	else if (WIFSIGNALED(exec->status))
	{
		signo = WTERMSIG(exec->status);
		if (signo == SIGPIPE)
		{
			// gestionar el SIGPIPE para que en vez de 141 salga 0
			if (minishell)
				minishell->exit = 0;
			else
				minishell->exit = 0;
			// Opcional: No imprimir nada
		}
		else if (signo == SIGINT)
		{
			if (minishell)
				minishell->exit = 130;
			write(1, "\n", 1);
		}
		else if (signo == SIGQUIT)
		{
			if (minishell)
				minishell->exit = 131;
			write(1, "Quit (core dumped)\n", 19);
		}
		else
		{
			if (minishell)
				minishell->exit = 128 + signo;
		}
	}
	if (g_signal == S_SIGINT_CMD)
	{
		if (minishell)
			minishell->exit = 130;
	}
	g_signal = S_BASE;
	return (minishell ? minishell->exit : 0);
} */
/*
int	execute_command_list(t_minishell *minishell)
{
	t_command	*current;

	current = minishell->cmd;
	while (current)
	{
		if (ft_strcmp(current->args[0], "&&") == 0)
		{
			if (minishell->exit != 0)
				current = current->next;
			current = current->next;
			continue ;
		}
		else if (ft_strcmp(current->args[0], "||") == 0)
		{
			if (minishell->exit == 0)
				current = current->next;
			current = current->next;
			continue ;
		}
		ft_check_executor_single(minishell, current);
		current = current->next;
	}
	if (minishell)
		return (minishell->exit);
	return (0);
}*/

/* 
// *
//  * nueva con solo dos parametros
static void	execute_builtin_no_pipe(t_minishell *minishell, t_command *cmd)
{
	int			saved_stdin;
	int			saved_stdout;
	int			builtin_result;
	t_executor	*exec;

	exec = minishell->executor;
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (cmd->input_file != -1)
	{
		dup2(cmd->input_file, STDIN_FILENO);
		close(cmd->input_file);
	}
	if (cmd->output_file != -1)
	{
		dup2(cmd->output_file, STDOUT_FILENO);
		close(cmd->output_file);
	}
	builtin_result = execute_builtin(exec->builtin_id, cmd->args, exec->envp,
			exec->myenv);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	minishell->exit = builtin_result;
}

// *
//  * version nueva, solo 3 parametros
static void	child_process(t_minishell *minishell, t_command *cmd, int pipefd[2])
{
	char		*path;
	t_executor	*exec;

	exec = minishell->executor;
	path = NULL;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (cmd->input_file != -1)
	{
		dup2(cmd->input_file, STDIN_FILENO);
		close(cmd->input_file);
	}
	else if (exec->prev_fd != -1)
	{
		dup2(exec->prev_fd, STDIN_FILENO);
		close(exec->prev_fd);
	}
	if (cmd->output_file != -1)
	{
		dup2(cmd->output_file, STDOUT_FILENO);
		close(cmd->output_file);
	}
	else if (cmd->operator== PIPE)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
	}
	if (exec->builtin_id != -1)
		exit(execute_builtin(exec->builtin_id, cmd->args, exec->envp,
				exec->myenv));
	if (cmd->args[0][0] == '/' || cmd->args[0][0] == '.')
		path = ft_strdup(cmd->args[0]);
	else
		path = get_path(cmd->args[0], exec->envp);
	if (!path || access(path, X_OK) != 0)
	{
		ft_putstr_fd(cmd->args[0], 2);
		write(2, ": command not found\n", 21);
		free(path);
		exit(EXIT_CMD_NOT_FOUND);
	}
	execve(path, cmd->args, exec->myenv->env);
	exit(EXIT_CMD_NOT_FOUND);
}

// *
//  * version nueva menos parametros
static void	parent_process(t_minishell *minishell, t_command *cmd,
	int pipefd[2], pid_t pid)
{
int			status;
int			signo;
t_executor	*exec;

exec = minishell->executor;
if (exec->prev_fd != -1)
	close(exec->prev_fd);
if (cmd->operator== PIPE)
{
	close(pipefd[1]);
	exec->prev_fd = pipefd[0];
}
else
	exec->prev_fd = -1;
waitpid(pid, &status, 0);
if	(WIFEXITED(status))
	minishell->exit = WEXITSTATUS(status);
else if	(WIFSIGNALED(status))
{
	signo = WTERMSIG(status);
	if (signo == SIGPIPE)
		minishell->exit = 0;
	else if (signo == SIGINT)
	{
		minishell->exit = 130;
		write(1, "\n", 1);
	}
	else if (signo == SIGQUIT)
	{
		minishell->exit = 131;
		write(1, "Quit (core dumped)\n", 19);
	}
	else
		minishell->exit = 128 + signo;
}
g_signal = S_BASE;
}

// *
//  * nueva funcion recibiendo solo 2 parametros
void	ft_check_executor_single(t_minishell *minishell, t_command *cmd)
{
	int			pipefd[2];
	pid_t		pid;
	t_executor	*exec;

	exec = minishell->executor;
	if (cmd->limiter)
		ft_here_doc(cmd->limiter);
	if (!cmd->args || !cmd->args[0])
		return ;
	exec->builtin_id = get_builtin_cmd(cmd->args[0]);
	if (exec->builtin_id != -1 && cmd->operator!= PIPE && exec->prev_fd == - 1)
	{
		execute_builtin_no_pipe(minishell, cmd);
		return ;
	}
	if (cmd->operator== PIPE)
	{
		if (pipe(pipefd) == -1)
			ft_exit("pipe failed");
	}
	g_signal = S_CMD;
	pid = fork();
	if (pid == -1)
		ft_exit("fork failed");
	if (pid == 0)
		child_process(minishell, cmd, pipefd);
	else
		parent_process(minishell, cmd, pipefd, pid);
}

// *
//  * nueva funcion de executer recibiendo solo un parametro
int	execute_command_list(t_minishell *minishell)
{
	t_command	*current;

	current = minishell->cmd;
	while (current)
	{
		if (ft_strcmp(current->args[0], "&&") == 0)
		{
			if (minishell->exit != 0)
				current = current->next;
			current = current->next;
			continue ;
		}
		else if (ft_strcmp(current->args[0], "||") == 0)
		{
			if (minishell->exit == 0)
				current = current->next;
			current = current->next;
			continue ;
		}
		ft_check_executor_single(minishell, current);
		current = current->next;
	}
	if (minishell)
		return (minishell->exit);
	return (0);
} */

/**
 * version vieja con 5 parametros
 */
/*
static void	execute_builtin_no_pipe(t_minishell *minishell, t_executor *exec,
		t_command *cmd, char **envp, t_myenv *myenv)
{
	int	saved_stdin;
	int	saved_stdout;
	int	builtin_result;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (cmd->input_file != -1)
	{
		dup2(cmd->input_file, STDIN_FILENO);
		close(cmd->input_file);
	}
	if (cmd->output_file != -1)
	{
		dup2(cmd->output_file, STDOUT_FILENO);
		close(cmd->output_file);
	}
	builtin_result = execute_builtin(exec->builtin_id, cmd->args, envp, myenv);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	minishell->exit = builtin_result;
}
 */

/**
 * version antigua, muchos parametros
 */
/*
static void	child_process(t_executor *exec, t_command *cmd, char **envp,
		t_myenv *myenv, int pipefd[2])
{
	char	*path;

	path = NULL;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (cmd->input_file != -1)
	{
		dup2(cmd->input_file, STDIN_FILENO);
		close(cmd->input_file);
	}
	else if (exec->prev_fd != -1)
	{
		dup2(exec->prev_fd, STDIN_FILENO);
		close(exec->prev_fd);
	}
	if (cmd->output_file != -1)
	{
		dup2(cmd->output_file, STDOUT_FILENO);
		close(cmd->output_file);
	}
	else if (cmd->operator== PIPE)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
	}
	if (exec->builtin_id != -1)
		exit(execute_builtin(exec->builtin_id, cmd->args, envp, myenv));
	if (cmd->args[0][0] == '/' || cmd->args[0][0] == '.')
		path = ft_strdup(cmd->args[0]);
	else
		path = get_path(cmd->args[0], envp);
	if (!path || access(path, X_OK) != 0)
	{
		ft_putstr_fd(cmd->args[0], 2);
		write(2, ": command not found\n", 21);
		free(path);
		exit(EXIT_CMD_NOT_FOUND);
	}
	execve(path, cmd->args, myenv->env);
	exit(EXIT_CMD_NOT_FOUND);
}
 */

/**
 * version vieja demasiados parametros
 */
/*
static void	parent_process(t_minishell *minishell, t_executor *exec,
		t_command *cmd, int pipefd[2], pid_t pid)
{
	int	status;
	int	signo;

	if (exec->prev_fd != -1)
		close(exec->prev_fd);
	if (cmd->operator== PIPE)
	{
		close(pipefd[1]);
		exec->prev_fd = pipefd[0];
	}
	else
		exec->prev_fd = -1;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		minishell->exit = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		signo = WTERMSIG(status);
		if (signo == SIGPIPE)
			minishell->exit = 0;
		else if (signo == SIGINT)
		{
			minishell->exit = 130;
			write(1, "\n", 1);
		}
		else if (signo == SIGQUIT)
		{
			minishell->exit = 131;
			write(1, "Quit (core dumped)\n", 19);
		}
		else
			minishell->exit = 128 + signo;
	}
	g_signal = S_BASE;
}
 */

/**
 * vieja funcion recibiendo 5 parametros
 */
/*
void	ft_check_executor_single(t_minishell *minishell, t_executor *exec,
		t_command *cmd, char **envp, t_myenv *myenv)
{
	int		pipefd[2];
	pid_t	pid;

	if (cmd->limiter)
		ft_here_doc(cmd->limiter);
	if (!cmd->args || !cmd->args[0])
		return ;
	exec->builtin_id = get_builtin_cmd(cmd->args[0]);
	if (exec->builtin_id != -1 && cmd->operator!= PIPE && exec->prev_fd == - 1)
	{
		execute_builtin_no_pipe(minishell, exec, cmd, envp, myenv);
		return ;
	}
	if (cmd->operator == PIPE)
	{
		if (pipe(pipefd) == -1)
			ft_exit("pipe failed");
	}
	g_signal = S_CMD;
	pid = fork();
	if (pid == -1)
		ft_exit("fork failed");
	if (pid == 0)
		child_process(exec, cmd, envp, myenv, pipefd);
	else
		parent_process(minishell, exec, cmd, pipefd, pid);
}
 */

/**
 * funcion antigua con 5 parametros de entrada, se cambia para
 * que solo reciba la estructura t_minishell
 */
/*
int	execute_command_list(t_minishell *minishell, t_executor *exec,
		t_command *cmd, char **envp, t_myenv *myenv)
{
	t_command	*current;

	current = cmd;
	while (current)
	{
		if (ft_strcmp(current->args[0], "&&") == 0)
		{
			if (minishell->exit != 0)
				current = current->next;
			current = current->next;
			continue ;
		}
		else if (ft_strcmp(current->args[0], "||") == 0)
		{
			if (minishell->exit == 0)
				current = current->next;
			current = current->next;
			continue ;
		}
		ft_check_executor_single(minishell, exec, current, envp, myenv);
		current = current->next;
	}
	if (minishell)
		return (minishell->exit);
	return (0);
}
 */
void	redirections(t_command *cmd, int *saved_stdin, int *saved_stdout)
{
	if (cmd->input_file != -1)
	{
		*saved_stdin = dup(STDIN_FILENO);
		dup2(cmd->input_file, STDIN_FILENO);
		close(cmd->input_file);
	}
	if (cmd->output_file != -1)
	{
		*saved_stdout = dup(STDOUT_FILENO);
		dup2(cmd->output_file, STDOUT_FILENO);
		close(cmd->output_file);
	}
}

void	restore_redirections(int saved_stdin, int saved_stdout)
{
	if (saved_stdin != -1)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
	}
	if (saved_stdout != -1)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
}
#include <stdio.h>

void	print_args(char **args)
{
	int	i;

	i = 0;
	while (args[i] != NULL)
	{
		printf("Argument %d: %s\n", i, args[i]);
		i++;
	}
}
