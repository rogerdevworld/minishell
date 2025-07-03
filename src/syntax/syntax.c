/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:05:47 by rmarrero          #+#    #+#             */
/*   Updated: 2025/04/22 20:16:44 by xviladri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

// -- serparacion general del bonus por lista de comandos -- //
t_ast_node	*build_ast(t_command *cmd_list)
{
	t_ast_node	*node;

	if (!cmd_list)
		return (NULL);
	node = malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	if (cmd_list->operator== COMMAND || cmd_list->operator== NONE)
	{
		node->op = COMMAND;
		node->cmd = cmd_list;
		node->left = NULL;
		node->right = NULL;
		return (node);
	}
	else
	{
		node->op = cmd_list->operator;
		node->cmd = cmd_list;
		node->left = build_ast_without_operator(cmd_list);
		node->right = build_ast(cmd_list->next);
	}
	return (node);
}

// Liberar recursivamente todo el AST
void	free_ast(t_ast_node *root)
{
	if (!root)
		return ;
	free_ast(root->left);
	free_ast(root->right);
	// No liberamos root->cmd porque ese puntero está gestionado fuera (por el parser)
	free(root);
}

// Función auxiliar para imprimir indentación
static void	print_indent(int level)
{
	for (int i = 0; i < level; i++)
		printf("  ");
}

// Imprimir el AST de forma recursiva con niveles
void	print_ast(t_ast_node *root, int level)
{
	int	i;

	if (!root)
		return ;
	print_indent(level);
	printf("Nodo AST - operador: %s\n", operator_to_str(root->op));
	if (root->cmd && root->op == COMMAND)
	{
		print_indent(level + 1);
		printf("Comando: %s\n", root->cmd->args[0]);
		i = 1;
		while (root->cmd->args[i])
		{
			print_indent(level + 1);
			printf("Arg[%d]: %s\n", i, root->cmd->args[i]);
			i++;
		}
		// Puedes añadir impresión de redirecciones si quieres
		if (root->cmd->input_file != -1)
		{
			print_indent(level + 1);
			printf("Input FD: %d\n", root->cmd->input_file);
		}
		if (root->cmd->output_file != -1)
		{
			print_indent(level + 1);
			printf("Output FD: %d\n", root->cmd->output_file);
		}
		if (root->cmd->limiter)
		{
			print_indent(level + 1);
			printf("Here_doc limiter: %s\n", root->cmd->limiter);
		}
	}
	if (root->left)
	{
		print_indent(level);
		printf("Left:\n");
		print_ast(root->left, level + 1);
	}
	if (root->right)
	{
		print_indent(level);
		printf("Right:\n");
		print_ast(root->right, level + 1);
	}
}
t_ast_node *build_ast_without_operator(t_command *cmd_list)
{
	t_ast_node	*root = NULL;
	t_ast_node	*current = NULL;
	t_ast_node	*new_node;

	while (cmd_list)
	{
		new_node = malloc(sizeof(t_ast_node));
		if (!new_node)
		{
			free_ast(root);
			return (NULL);
		}
		new_node->op = COMMAND;
		new_node->cmd = cmd_list;
		new_node->left = NULL;
		new_node->right = NULL;

		if (!root)
		{
			root = new_node;
			current = new_node;
		}
		else
		{
			// Como no hay operador, vamos enlazando por derecha como lista simple (opcional)
			current->right = new_node;
			current = new_node;
		}

		cmd_list = cmd_list->next;
	}

	return (root);
}