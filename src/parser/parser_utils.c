/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xviladri <xviladri@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:43:57 by xviladri          #+#    #+#             */
/*   Updated: 2025/04/01 14:11:55 by xviladri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

t_command       *init_command(void)
{
        t_command       *cmd;

        cmd = (t_command *)malloc(sizeof(t_command));
        if (!cmd)
                return (NULL);
        cmd->args = ft_calloc(10, sizeof(char *));
        if (!cmd->args)
                return (free(cmd), NULL);
        cmd->path = NULL;
        cmd->input_file = NULL;
        cmd->output_file = NULL;
        cmd->append = 0;
        cmd->operator = NONE;
        cmd->next = NULL;
        return (cmd);
}

void    handle_redirect(t_command *cmd, t_token **tokens)
{
        if (ft_strcmp((*tokens)->value, ">") == 0)
        {
                *tokens = (*tokens)->next;
                if (!(*tokens) || !(*tokens)->value)
                        return ;//error:falta archivo para redirigir.
                cmd->output_file = ft_strdup((*tokens)->value);
                cmd->append = 0;
        }
        else if (ft_strcmp((*tokens)->value, ">>") == 0)
        {
                *tokens = (*tokens)->next;
                if (!(*tokens) || !(*tokens)->value)
                        return ;//error:falta archivo para redirigir.
                cmd->output_file = ft_strdup((*tokens)->value);
                cmd->append = 1;
        }
        else if (ft_strcmp((*tokens)->value, "<") == 0)
        {
                *tokens = (*tokens)->next;
                if (!(*tokens) || !(*tokens)->value)
                        return ;//error:falta archivo para redirigir.
                cmd->input_file = ft_strdup((*tokens)->value);
                cmd->append = 0;
        }
}

int     handle_first_argument(t_token *tokens, t_command *current)
{
       current = current;
       	if (tokens->type == TOKEN_COMMAND)
        {
                if (tokens->value[0] == '\n')//si 1er token es un '\n', lo interpreto como cmd vacío.
                        return (0);
        }
        return (1);//el 1er argum es valido.
}

void    handle_operator(t_token *tokens, t_command **current)// -- estoy haciendo un arbol de ejecucion para ver grupo de comandso orden etc -- //
{
        (*current)->operator = resolve_operator(tokens->value);
        (*current)->next = init_command();
        *current = (*current)->next;
}
