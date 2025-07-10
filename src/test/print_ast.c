#include "../../include/minishell.h"

void print_ast(t_ast *node, int depth)
{
    int i = 0;
    while (i++ < depth)
        printf("  ");

    if (!node)
    {
        printf("(null)\n");
        return;
    }

    if (node->type == NODE_COMMAND)
    {
        printf("COMMAND:\n");
        print_command(node->cmd);
    }
    else if (node->type == NODE_PIPE)
        printf("PIPE\n");
    else if (node->type == NODE_AND)
        printf("AND\n");
    else if (node->type == NODE_OR)
        printf("OR\n");
    else if (node->type == NODE_SUBSHELL)
        printf("SUBSHELL\n");

    print_ast(node->left, depth + 1);
    print_ast(node->right, depth + 1);
}