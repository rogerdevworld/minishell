#include "../../include/minishell.h"

void	print_command(t_command *cmd)
{
	if (!cmd)
	{
		printf("Command is NULL\n");
		return ;
	}
	printf("         Command:\n");
	// Imprimir args
	if (cmd->args[0])
	{
		printf("           args: ");
		for (int i = 0; cmd->args[i]; i++)
			printf(" \"%s\"", cmd->args[i]);
		printf("\n");
	}
	else
		printf("           args: (null)\n");
	// Imprimir path
	printf("           path: %s\n", cmd->path ? cmd->path : " (null)");
	// Imprimir file descriptors
	printf("           input_file fd: %d\n", cmd->redir->input_file);
	printf("           output_file fd: %d\n", cmd->redir->output_file);
	// name redir
	printf("         Infile:\n");
	if (cmd->redir->in_file)
	{
		printf("           Infile                               :");
		for (int i = 0; cmd->redir->in_file[i] != NULL; i++)
			printf(" \"%s\"", cmd->redir->in_file[i]);
		printf("\n");
	}
	else
		printf("           Infile: (null)\n");
	// name redir
	printf("         Outfile:\n");
	if (cmd->redir->out_file)
	{
		printf("           Outfile:");
		for (int i = 0; cmd->redir->out_file[i] != NULL; i++)
			printf(" \"%s\"", cmd->redir->out_file[i]);
		printf("\n");
	}
	else
		printf("           Outfile: (null)\n");
	// Imprimir limiter si existe
	printf("         Limiter:\n");
	if (cmd->redir->limiter)
	{
		printf("           args:");
		for (int i = 0; cmd->redir->limiter[i] != NULL; i++)
			printf(" \"%s\"", cmd->redir->limiter[i]);
		printf("\n");
	}
	else
		printf("           Limiter: (null)\n");
}
