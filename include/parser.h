/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xviladri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:56:13 by rmarrero          #+#    #+#             */
/*   Updated: 2025/03/27 11:57:56 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef PARSER_H
# define PARSER_H

// -- parser.h -- //
typedef struct s_command
{
	char **args;            // guardamos comandos (["echo", "hola"])
	char *path;             // path del comando
	char *input_file;       // archivo entrada por si hay "<"
	char *output_file;      // archivo salida por si hay ">"
	int append;             // 1 si es ">>", 0 si es ">"
	struct s_command *next; // para encadenar comandos cuando hay un pipe "|| && -" falta ver si es un file el siguente comando ejemplo "cat -e file"
}			t_command;

t_command	*init_command(void);
void		handle_redirect(t_command *cmd, t_token **tokens);
t_command	*parse_tokens(t_token *tokens, char **envp);
void		print_command_list(t_command *cmds);

char	*get_path(char *cmd, char **env);

#endif