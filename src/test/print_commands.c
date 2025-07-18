#include "../../include/minishell.h"

void print_command(t_command *cmd)
{
    if (!cmd)
    {
        printf("Command is NULL\n");
        return;
    }

    printf("         Command:\n");

    // Imprimir args
    if (cmd->args)
    {
        printf("           args:");
        for (int i = 0; cmd->args[i] != NULL; i++)
            printf(" \"%s\"", cmd->args[i]);
        printf("\n");
    }
    else
        printf("           args: (null)\n");

    // Imprimir path
    printf("           path: %s\n", cmd->path ? cmd->path : "         (null)");

    // Imprimir file descriptors
    printf("           input_file fd: %d\n", cmd->redir->input_file);
    printf("           output_file fd: %d\n", cmd->redir->output_file);

    //Imprimir limiter si existe
    if (cmd->redir->limiter)
        printf("           limiter: \"%s\"\n", cmd->redir->limiter[0]);
    else
        printf("           limiter: (null)\n");
}
