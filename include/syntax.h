#ifndef SYNTAX_H
# define SYNTAX_H

# include "minishell.h" // para tener t_command, t_operator y demás

typedef struct s_ast_node
{
	char *op;                 // operador lógico o PIPE o COMMAND
	t_command *cmd;           // comando o NULL si es nodo operador
	struct s_ast_node *left;  // hijo izquierdo (comando o subárbol)
	struct s_ast_node *right; // hijo derecho (comando o subárbol)
}			t_ast_node;

// Construcción del AST desde la lista enlazada de comandos
t_ast_node	*build_ast(t_command *cmd_list);

// Liberar el AST completo
void		free_ast(t_ast_node *root);

// Funciones auxiliares para ejecución o impresión (opcional)
void		print_ast(t_ast_node *root, int level);

t_ast_node	*build_ast_without_operator(t_command *cmd_list);

#endif
