/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xviladri <xviladri@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:43:57 by xviladri          #+#    #+#             */
/*   Updated: 2025/04/01 14:11:55 by xviladri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

t_command	*init_command(void)
{
	t_command	*cmd;

	cmd = (t_command *)malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = ft_calloc(10, sizeof(char *));
	cmd->path = NULL;
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->append = 0;
	cmd->next = NULL;
	return (cmd);
}

void	handle_redirect(t_command *cmd, t_token **tokens)
{
	if (ft_strcmp((*tokens)->value, ">") == 0)
	{
		*tokens = (*tokens)->next;
		cmd->output_file = ft_strdup((*tokens)->value);
		cmd->append = 0;
	}
	else if (ft_strcmp((*tokens)->value, ">>") == 0)
	{
		*tokens = (*tokens)->next;
		cmd->output_file = ft_strdup((*tokens)->value);
		cmd->append = 1;
	}
	else if (ft_strcmp((*tokens)->value, "<") == 0)
	{
		*tokens = (*tokens)->next;
		cmd->input_file = ft_strdup((*tokens)->value);
		cmd->append = 0;
	}
}

t_bonus	*parse_tokens(t_token *tokens, char **envp)
{
	t_command	*cmds;
	t_command	*current;
	t_bonus		*root = NULL;
	t_bonus		*prev = NULL;
	t_bonus		*next = NULL;
	t_bonus		*node;
	int			i;

	envp = envp;
	cmds = init_command();
	if (!cmds)
		return (NULL);
	current = cmds;
	i = 0;
	while (tokens)
	{
		// -- falta agregar caso de si "argumento es el primer parametro no aceptar", NO TIENE QUE HACER \n --//
		if ((tokens->type == TOKEN_COMMAND) || (tokens->type == TOKEN_ARGUMENT))
		{
			current->args[i] = ft_strdup(tokens->value);
			// -- el path de args[0] simpre sera asi pero eso el 0 esta statico -- //
			current->path = get_path(current->args[0], envp);
			i++;
		}
		else if (tokens->type == TOKEN_REDIRECTION)
			handle_redirect(current, &tokens);
		else if (tokens->type == TOKEN_OPERATOR)
		{
			node = init_bonus(resolve_operator(tokens->value), current, prev, next);
			prev = node;
			if (!root)
				root = node;
			current->next = init_command();
			current = current->next;
			i = 0;
		}
		tokens = tokens->next;
	}
	if (!root)
	{
		root = init_bonus(COMMAND, cmds, NULL, NULL);
	}
	return (root);
}

void	print_operator(t_operator op)
{
	if (op == PIPE)
		ft_printf("Operador: |\n");
	else if (op == AND)
		ft_printf("Operador: &&\n");
	else if (op == OR)
		ft_printf("Operador: ||\n");
	else if (op == COMMAND)
		ft_printf("Operador: NONE\n");
	else
		ft_printf("Operador: UNKNOWN (%d)\n", op);
}

void	print_command(t_command *cmd)
{
	int	i = 0;

	if (!cmd)
	{
		ft_printf("  [Comando vacío]\n");
		return;
	}

	while (cmd)
	{
		ft_printf("  Comando:\n");
		while (cmd->args && cmd->args[i])
		{
			ft_printf("    Arg[%d]: %s\n", i, cmd->args[i]);
			i++;
		}
		if (cmd->path)
			ft_printf("    Path: %s\n", cmd->path);
		if (cmd->input_file)
			ft_printf("    Input: %s\n", cmd->input_file);
		if (cmd->output_file)
			ft_printf("    Output: %s (append: %d)\n", cmd->output_file, cmd->append);
		ft_printf("Nodo: %p", cmd);
		cmd = cmd->next;
	}
}

void	print_bonus_tree(t_bonus *node, int level)
{
	if (!node)
		return;
	print_bonus_tree(node->right, level + 1);
	for (int i = 0; i < level; i++)
		ft_printf("    ");
	print_operator(node->type);
	print_command(node->cmd);
	print_bonus_tree(node->left, level + 1);
}

// -- cosas por aplciar -- //
// 1. free de strcuts
// 2. comillas solas simpre en parejas o dentro de una pareja 
// 3. si pasan un separador ; es un error
// 4. si pasan operadores en formato error();
// 4.1 ejemplo >&& >||, > &&||, cosas asi
// 5. 