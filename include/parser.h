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

typedef enum s_operator
{
	COMMAND,
	PIPE,
	AND,
	OR,
	NONE
}						t_operator;

typedef struct s_command
{
	char				**args;
	char				*path;
	char				*input_file;
	char				*output_file;
	int					append;
	t_operator			operator;
	struct s_command	*next;
}						t_command;

// guardamos comandos (["echo", "hola"])
// path del comando
// archivo entrada por si hay "<"
// archivo salida por si hay ">"
// 1 si es ">>", 0 si es ">"
// para encadenar comandos cuando hay un pipe "|| &&
//" falta ver si es un file el siguente comando ejemplo "cat -e file"
//
//parser_utils.c
t_command				*init_command(void);
void					handle_redirect(t_command *cmd, t_token **tokens);
int					handle_first_argument(t_token *tokens, t_command *current);
void					handle_operator(t_token *tokens, t_command **current);
//parser.c
char					**process_cmd_args(t_token *tokens, char **envp, char **args);
t_command				*parse_tokens(t_token *tokens, char **envp);
void					print_command_list(t_command *cmds);
t_operator				resolve_operator(char *operator);
const char*				operator_to_str(t_operator op);
//get_path.c
char					*get_path(char *cmd, char **env);
//ft_isoperator.c
int					ft_isoperator(char *str);
#endif
