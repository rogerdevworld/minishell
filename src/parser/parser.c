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

char	**process_cmd_args(t_token *tokens, char **envp, char **args)
{
	int	i;

	envp = envp;
	i = 0;
	while (tokens && (tokens->type == TOKEN_COMMAND || tokens->type == TOKEN_ARGUMENT))
	{
		if (i >= 10)//evitar desbord.
			break ;
		args[i] = ft_strdup(tokens->value);
		tokens = tokens->next;
		i++;
	}
	return (args);
}

t_command	*parse_tokens(t_token *tokens, char **envp)
{
	t_command	*cmds;
	t_command	*current;

	cmds = init_command();
	if (!cmds)
		return (NULL);
	current = cmds;
	while (tokens)
	{
		if ((tokens->type == TOKEN_COMMAND) || (tokens->type == TOKEN_ARGUMENT))
		{
			if(!handle_first_argument(tokens, current))// -- falta agregar caso de si "argumento es el primer parametro no aceptar", NO TIENE QUE HACER \n --//
				return (NULL);
			current->args = process_cmd_args(tokens, envp, current->args);
			if (current->args[0] && !current->path)// -- el path de args[0] simpre sera asi pero eso el 0 esta statico -- (xeni: asi asginamos la ruta solo al 1er argum(cmd)?)//
				current->path = get_path(current->args[0], envp);
		}
		else if (tokens->type == TOKEN_REDIRECTION)
			handle_redirect(current, &tokens);
		else if (tokens->type == TOKEN_OPERATOR)
			handle_operator(tokens, &current);
		tokens = tokens->next;
	}
	return (cmds);
}
/*
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
			ft_printf("----comandos: %s - path: %s\n", cmds->args[i], cmds->path);
			i++;
		}
		k++;
		ft_printf("\n");
		if (cmds->input_file)
			ft_printf("  Input: %s\n", cmds->input_file);
		if (cmds->output_file)
			ft_printf("  Output: %s (append: %d)\n", cmds->output_file,
				cmds->append);
		cmds = cmds->next;
	}
}*/

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
/*
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
    else
        return "UNKNOWN";
    return (cmd);
}*/
