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
	cmd->input_file = -1;
	cmd->output_file = -1;
	cmd->limiter = NULL;
	cmd->operator= NONE;
	cmd->next = NULL;
	return (cmd);
}

void	handle_redirect(t_command *cmd, t_token **tokens)
{
	t_token	*tmp;

	if (ft_strncmp((*tokens)->value, ">", 1) == 0)
	{
		*tokens = (*tokens)->next;
		cmd->output_file = ft_open((*tokens)->value, 1);
		// cmd->append = 0;
	}
	else if (ft_strncmp((*tokens)->value, ">>", 2) == 0
		|| ft_strncmp((*tokens)->value, "<<", 2) == 0)
	{
		// -- tenemos que controlar el limitador del here_doc
		*tokens = (*tokens)->next;
		tmp = get_last_token(*tokens);
		cmd->output_file = ft_open(tmp->value, 2);
		cmd->limiter = ft_strdup((*tokens)->value);
	}
	else if (ft_strncmp((*tokens)->value, "<", 1) == 0)
	{
		*tokens = (*tokens)->next;
		cmd->input_file = ft_open((*tokens)->value, 0);
		// cmd->append = 0;
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
		// -- falta agregar caso de si "argumento es el primer parametro no aceptar",
		//	NO TIENE QUE HACER \n --//
		if ((tokens->type == TOKEN_COMMAND) || (tokens->type == TOKEN_ARGUMENT))
		{
			current->args[i] = ft_strdup(tokens->value);
			// -- el path de args[0] simpre sera asi pero eso el 0 esta statico --
				//
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
			// -- estoy haciendo un arbol de ejecucion para ver grupo de comandso orden etc -- //
			current->operator= resolve_operator(tokens->value);
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
t_operator	resolve_operator(char *operator)
{
	if (ft_strncmp(operator, "||", 2) == 0)
		return (OR);
	if (ft_strcmp(operator, "&&") == 0)
		return (AND);
	if (ft_strcmp(operator, "|") == 0)
		return (PIPE);
	return (COMMAND);
}
const char	*operator_to_str(t_operator op)
{
	if (op == PIPE)
		return ("|");
	else if (op == AND)
		return ("&&");
	else if (op == OR)
		return ("||");
	else if (op == COMMAND)
		return ("COMMAND");
	else
		return ("UNKNOWN");
}