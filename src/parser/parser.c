/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xviladri <xviladri@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:43:57 by xviladri          #+#    #+#             */
/*   Updated: 2025/04/22 20:24:20 by xviladri         ###   ########.fr       */
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
	if (!tokens || !*tokens || !(*tokens)->next)
		return ;

	if (ft_strncmp((*tokens)->value, ">>", 2) == 0)
	{
		*tokens = (*tokens)->next;
		cmd->output_file = ft_open((*tokens)->value, 2); // modo append
	}
	else if (ft_strncmp((*tokens)->value, "<<", 2) == 0)
	{
		*tokens = (*tokens)->next;
		cmd->limiter = ft_strdup((*tokens)->value); // guardar el limitador
		cmd->input_file = ft_open(cmd->limiter, 2); // simula heredoc
	}
	else if (ft_strncmp((*tokens)->value, ">", 1) == 0)
	{
		*tokens = (*tokens)->next;
		cmd->output_file = ft_open((*tokens)->value, 1); // modo trunc
	}
	else if (ft_strncmp((*tokens)->value, "<", 1) == 0)
	{
		*tokens = (*tokens)->next;
		cmd->input_file = ft_open((*tokens)->value, 0); // modo lectura
	}
}

t_command	*parse_tokens(t_token *tokens, char **envp)
{
	t_command	*cmds;
	t_command	*current;
	int			i;
	char		*clean_arg;
	t_token		*prev;

	(void)envp;

	if (!tokens)
		return (NULL);
	if (tokens->type == TOKEN_OPERATOR)//caso 1. si primer argum (token) es un operador (&& o ||).
	{
		ft_exit("syntax error near unexpected token");
		return (NULL);
	}
	cmds = init_command();
	if (!cmds)
		return (NULL);
	current = cmds;
	i = 0;
	prev = NULL;

	while (tokens)
	{
		if (tokens->type == TOKEN_OPERATOR && prev && prev->type == TOKEN_OPERATOR)
		{
			ft_exit("syntax error near unexpected token");
			return (NULL);
		}
		else if (tokens->type == TOKEN_OPERATOR && !tokens->next)
		{
			ft_exit("syntax error near unexpected token");
			return (NULL);
		}
		else if ((tokens->type == TOKEN_COMMAND) || (tokens->type == TOKEN_ARGUMENT))
		{
			//printf("TOKEN: %s (tipo %d)\n", tokens->value, tokens->type);
			//printf("ARG[%d]: %s\n", i, current->args[i]);
			clean_arg = remove_quotes(tokens->value);//para quitar las comillas.
			current->args[i] = clean_arg;
			// -- el path de args[0] simpre sera asi pero eso el 0 esta statico --
				//
			//free(clean_arg);
			current->path = get_path(current->args[0], envp);
			i++;
		}
		else if (tokens->type == TOKEN_REDIRECTION)
		{
			handle_redirect(current, &tokens);
			if (tokens)
				tokens = tokens->next;
			prev = tokens;
			continue ;
		}
		else if (tokens->type == TOKEN_OPERATOR)
		{
			// -- estoy haciendo un arbol de ejecucion para ver grupo de comandso orden etc --
				//
			current->next = init_command();
			if (!current->next)
				return (NULL);
			current = current->next;
			current->args[0] = ft_strdup(operator_to_str(resolve_operator(tokens->value)));
			current->operator = resolve_operator(tokens->value);
			current->next = init_command();
			if (!current->next)
				return (NULL);
			current = current->next;
			i = 0;
		}
		prev = tokens;
		tokens = tokens->next;
	}
	return (cmds);
}

void	print_command_list(t_command *cmds)
{
	int	i;
	int	j;
	int	k;

	k = 1;
	while (cmds)
	{
		i = 0;
		ft_printf("OPERADOR: %s\n", operator_to_str(cmds->operator));
		ft_printf("--comandos + flag %i: ", k);
		while (cmds->args[i])
		{
			j = 0;
			while (cmds->args[j])
			{
				ft_printf("%s -> ", cmds->args[j]);
				j++;
			}
			ft_printf("\n---Nodo: %p\n", cmds);
			ft_printf("----N: %i - comando: %s\n", i, cmds->args[i]);
			ft_printf("----comandos: %s - path: %s\n", cmds->args[i],
				cmds->path);
			if (cmds->limiter)
				ft_printf("------------------------------\nCaso here_doc: limiter: %s\n", cmds->limiter);
			ft_printf("infile: %i\n", cmds->input_file);
			ft_printf("outfile: %i\n", cmds->output_file);
			i++;
		}
		k++;
		// -- dejare el infile y el outfile como fd en la struct para nejarlos desde ahi solo
		// en forma de texto luego lo hago mejor para que se vea mas claro con parse bonus --
			//
		ft_printf("\n");
		// if (cmds->input_file)
		//	ft_printf("  Input: %s\n", cmds->input_file);
		// if (cmds->output_file)
		//	ft_printf("  Output: %s (append: %d)\n", cmds->output_file,
		//		cmds->append);
		cmds = cmds->next;
	}
}

t_operator	resolve_operator(char *operator)
{
	if (ft_strncmp(operator, "||", 2) == 0)
		return (OR);
	if (ft_strcmp(operator, "&&") == 0)
		return (AND);
	if (ft_strncmp(operator, "|", 1) == 0)
		return (PIPE);
	return (COMMAND);
}

const char* operator_to_str(t_operator op)
{
    if (op == PIPE)
        return "|";
    else if (op == AND)
        return "&&";
    else if (op == OR)
        return "||";
    else if (op == COMMAND)
        return "COMMAND";
    return "UNKNOWN";
}