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

static void execute_builtin_no_pipe(t_minishell *minishell, t_executor *exec, t_command *cmd, char **envp, t_myenv *myenv)
{
    int saved_stdin = dup(STDIN_FILENO);
    int saved_stdout = dup(STDOUT_FILENO);

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

    int builtin_result = execute_builtin(exec->builtin_id, cmd->args, envp, myenv);

    dup2(saved_stdin, STDIN_FILENO);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdin);
    close(saved_stdout);

    minishell->exit = builtin_result;
}

static void child_process(t_executor *exec, t_command *cmd, char **envp, t_myenv *myenv, int pipefd[2])
{
    char *path = NULL;

    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);

    // Input redirection
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

    // Output redirection
    if (cmd->output_file != -1)
    {
        dup2(cmd->output_file, STDOUT_FILENO);
        close(cmd->output_file);
    }
    else if (cmd->operator == PIPE)
    {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
    }

    // Builtin en child
    if (exec->builtin_id != -1)
        exit(execute_builtin(exec->builtin_id, cmd->args, envp, myenv));

    // Buscar PATH
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

static void parent_process(t_minishell *minishell, t_executor *exec, t_command *cmd, int pipefd[2], pid_t pid)
{
    int status;

    if (exec->prev_fd != -1)
        close(exec->prev_fd);

    if (cmd->operator == PIPE)
    {
        close(pipefd[1]);
        exec->prev_fd = pipefd[0];
    }
    else
    {
        exec->prev_fd = -1;
    }

    waitpid(pid, &status, 0);

    if (WIFEXITED(status))
    {
        minishell->exit = WEXITSTATUS(status);
    }
    else if (WIFSIGNALED(status))
    {
        int signo = WTERMSIG(status);
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

void ft_check_executor_single(t_minishell *minishell, t_executor *exec, t_command *cmd, char **envp, t_myenv *myenv)
{
    int pipefd[2];
    pid_t pid;

    // Here-document si aplica
    if (cmd->limiter)
        ft_here_doc(cmd->limiter);

    // Comando vacío
    if (!cmd->args || !cmd->args[0])
        return;

    exec->builtin_id = get_builtin_cmd(cmd->args[0]);

    // Built-in sin pipe
    if (exec->builtin_id != -1 && cmd->operator != PIPE && exec->prev_fd == -1)
    {
        execute_builtin_no_pipe(minishell, exec, cmd, envp, myenv);
        return;
    }

    // Pipe
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
    {
        child_process(exec, cmd, envp, myenv, pipefd);
    }
    else
    {
        parent_process(minishell, exec, cmd, pipefd, pid);
    }
}


int execute_command_list(t_minishell *minishell, t_executor *exec, t_command *cmd, char **envp, t_myenv *myenv)
{
    t_command *current = cmd;

    while (current)
    {
        // Operador AND (&&)
        if (ft_strcmp(current->args[0], "&&") == 0)
        {
            if (minishell->exit != 0)
            {
                // Falló el anterior: saltar el próximo comando
                current = current->next;
            }
            current = current->next;
            continue;
        }
        // Operador OR (||)
        else if (ft_strcmp(current->args[0], "||") == 0)
        {
            if (minishell->exit == 0)
            {
                // Éxito anterior: saltar el próximo comando
                current = current->next;
            }
            current = current->next;
            continue;
        }

        // Ejecutar un único comando
        ft_check_executor_single(minishell, exec, current, envp, myenv);

        // Avanzar
        current = current->next;
    }

    return minishell->exit;
}
/**
 * a partir de aqui esto es funcional, la funcion ft_check_executor_single y execute_command_list
 * se complementan para recorrer cmd y ejecutarlo segun lo que se debe hacer, ahora arriba refactorizare
 * el codigo de ft_check_executor_single
 */
/*
void ft_check_executor_single(t_minishell *minishell, t_executor *exec, t_command *cmd, char **envp, t_myenv *myenv)
{
    int pipefd[2];
    int saved_stdin, saved_stdout;
    char *path;

    pid_t pid;
    int status;
    int builtin_result;

    // Aquí no hay while(cmd): es sólo un nodo

    // Here-document si aplica
    if (cmd->limiter)
        ft_here_doc(cmd->limiter);

    // Comando vacío: no hacer nada
    if (!cmd->args || !cmd->args[0])
        return;

    exec->builtin_id = get_builtin_cmd(cmd->args[0]);

    // Built-in sin pipe
    if (exec->builtin_id != -1 && cmd->operator != PIPE && exec->prev_fd == -1)
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

        builtin_result = execute_builtin(exec->builtin_id, cmd->args, envp, myenv);

        dup2(saved_stdin, STDIN_FILENO);
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdin);
        close(saved_stdout);

        minishell->exit = builtin_result;
        return;
    }

    // Pipe
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
    {
        // CHILD
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);

        // Input redir
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

        // Output redir
        if (cmd->output_file != -1)
        {
            dup2(cmd->output_file, STDOUT_FILENO);
            close(cmd->output_file);
        }
        else if (cmd->operator == PIPE)
        {
            close(pipefd[0]);
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[1]);
        }

        // Ejecutar built-in en child si hay
        if (exec->builtin_id != -1)
            exit(execute_builtin(exec->builtin_id, cmd->args, envp, myenv));

        // Buscar PATH
        path = NULL;
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
    else
    {
        // PARENT
        if (exec->prev_fd != -1)
            close(exec->prev_fd);

        if (cmd->operator == PIPE)
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
            int signo = WTERMSIG(status);
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
}

int execute_command_list(t_minishell *minishell, t_executor *exec, t_command *cmd, char **envp, t_myenv *myenv)
{
    t_command *current = cmd;

    while (current)
    {
        // Operador AND (&&)
        if (ft_strcmp(current->args[0], "&&") == 0)
        {
            if (minishell->exit != 0)
            {
                // Falló el anterior: saltar el próximo comando
                current = current->next;
            }
            current = current->next;
            continue;
        }
        // Operador OR (||)
        else if (ft_strcmp(current->args[0], "||") == 0)
        {
            if (minishell->exit == 0)
            {
                // Éxito anterior: saltar el próximo comando
                current = current->next;
            }
            current = current->next;
            continue;
        }

        // Ejecutar un único comando
        ft_check_executor_single(minishell, exec, current, envp, myenv);

        // Avanzar
        current = current->next;
    }

    return minishell->exit;
}
*/
/**comentando las funciones para unirlas ya que generaban un error
 * en el flujo de la ejecucion de comandos
 */
// -- ast bonus all --//
/*
int	execute_astint(t_minishell *minishell, t_executor *exec, t_command *cmd,
	char **envp, t_myenv *myenv)
{
	int	status;


	while (cmd)
	{
		if (minishell->exit == 0)
			ft_check_executor(minishell, exec, cmd, envp, myenv);
		else if (ft_strcmp(cmd->args[0], "&&") == 0 && minishell->exit == 0)
		{
			ft_printf("AND: %s\n", cmd->args[0]);
			cmd = cmd->next;
			ft_printf("AND: %s\n", cmd->args[0]);
			if (cmd)
				ft_check_executor(minishell, exec, cmd, envp, myenv);
			cmd = cmd->next;
		}
		else if (ft_strcmp(cmd->args[0], "||") == 0 && minishell->exit != 0)
		{
			ft_printf("OR: %s\n", cmd->args[0]);
			cmd = cmd->next;
			ft_printf("OR: %s\n", cmd->args[0]);
			if (cmd)
				ft_check_executor(minishell, exec, cmd, envp, myenv);
			cmd = cmd->next;
		}

		cmd = cmd->next;
	}
	return (minishell->exit);
}

int	ft_check_executor(t_minishell *minishell, t_executor *exec, t_command *cmd,
		char **envp, t_myenv *myenv)
{
	int		pipefd[2];
	int		i;
	int		saved_stdin;
	int		saved_stdout;
	char	*path;
	int		last_builtin_result;

	i = 0;
	pid_t pids[1024]; // Máximo número de comandos
	last_builtin_result = 0;
	// Inicializar la estructura exec si no está inicializada
	if (!exec)
		exec = init_exec(myenv);
	if (!exec)
		return (1);
	while (cmd)
	{
		if (cmd->limiter)
			ft_here_doc(cmd->limiter);
		if (!cmd->args || !cmd->args[0])
		{
			cmd = cmd->next;
			continue ;
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
			//if (minishell->ast_tree->op == AND && minishell->exit == 0)
			//{
			//	ft_printf("segundo comando");
			//}
			//else
			//	exit(127);
			if (cmd->operator== PIPE)
				close(pipefd[0]);
			if (exec->builtin_id != -1)
				exit(execute_builtin(exec->builtin_id, cmd->args, envp, myenv));
			path = NULL;
			if (cmd->args[0][0] == '/' || cmd->args[0][0] == '.')
				path = ft_strdup(cmd->args[0]);
			else
				path = get_path(cmd->args[0], envp);
			if (!path || access(path, X_OK) != 0)
			{
				ft_putstr_fd(cmd->args[0], 2);
				write(2, ": command not found\n", 30);
				free(path);
				exit(EXIT_CMD_NOT_FOUND);
			}
			execve(path, cmd->args, myenv->env);
			///perror("execve");
			exit(EXIT_CMD_NOT_FOUND);
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
	while (i--)
		waitpid(pids[i], &(exec->status), 0);
	if (i == 0)
		return (last_builtin_result);
	//ASI FUNCIONA
	//
	//if (WIFEXITED(exec->status))
	//{
	//	if (minishell)
	//		return (minishell->exit = WEXITSTATUS(exec->status));
	//	else
	//		return (WEXITSTATUS(exec->status));
	//}
	//else
	//	return (1);
	//cambio para integrar senales 
	if (WIFEXITED(exec->status))
	{
		// Actualiza el minishell->exit_status
		if (minishell)
			minishell->exit = WEXITSTATUS(exec->status);
		//else
		//	minishell->exit = WEXITSTATUS(exec->status);
	}
	else if (WIFSIGNALED(exec->status))
	{
		// Si el proceso terminó por una señal, puedes asignar un código especial
		//if (minishell)
		//	minishell->exit = 128 + WTERMSIG(exec->status);
			int signo = WTERMSIG(exec->status);
			if (signo == SIGPIPE)
			{
				// Puedes decidir qué hacer:
				// Por ejemplo: setear exit_status = 141 (128+13)
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

	//if (minishell && minishell->exit == 127)
	//{
	//	if (cmd && cmd->args && cmd->args[0])
	//		ft_printf("%s: %s\n", cmd->args[0], "Command not found");
	//	else
	//		ft_printf("minishell: %s\n", "Command not found");
	//}

	if (g_signal == S_SIGINT_CMD)
	{
		if (minishell)
			minishell->exit = 130;
	}

	g_signal = S_BASE;

	return (minishell ? minishell->exit : 0);

}
*/
// -- function to execute a command -- //
// --  de momento esta funcion esta fuera pero la dejare para comandos unicos --
/*
pid_t	external_command(t_command *cmd, t_executor *ex)
{
	if (cmd->operator== PIPE)
	{
		if (pipe(ex->p_fd) == -1)
			ft_exit("pipe failed");
	}
	ex->pid = fork();
	if (ex->pid == -1)
		ft_exit("fork failed");
	if (ex->pid == 0)
		child_process(cmd, ex);
	else
		parent_process(cmd, ex);
	return (ex->pid);
}
*/
// -- hay muchos parametros pero las funciones solo se les pasara dos struct t_shell
// y las struct de las pipes para esta parte aplicare esto mas adelante mini 2.0 --
//
/*
void	child_process(t_command *cmd, t_executor *ex)
{
	if (cmd->input_file != -1)
	{
		dup2(cmd->input_file, STDIN_FILENO);
		close(cmd->input_file);
	}
	else if (ex->prev_fd != -1)
	{
		dup2(ex->prev_fd, STDIN_FILENO);
		close(ex->prev_fd);
	}
	if (cmd->operator== PIPE)
	{
		close(ex->p_fd[0]);
		if (cmd->output_file != -1)
			close(ex->p_fd[1]);
		else
		{
			dup2(ex->p_fd[1], STDOUT_FILENO);
			close(ex->p_fd[1]);
		}
	}
	if (cmd->output_file != -1)
	{
		dup2(cmd->output_file, STDOUT_FILENO);
		close(cmd->output_file);
	}
	if (ex->builtin_id != -1)
		execute_builtin(ex->builtin_id, cmd->args, ex->envp, ex->myenv);
	else
		execve(get_path(cmd->args[0], ex->envp), cmd->args, ex->myenv->env);
	exit(0);
}

void	parent_process(t_command *cmd, t_executor *ex)
{
	if (ex->prev_fd != -1)
		close(ex->prev_fd);
	if (cmd->operator== PIPE)
	{
		close(ex->p_fd[1]);
		ex->prev_fd = ex->p_fd[0];
	}
	else
	{
		ex->prev_fd = -1;
	}
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
