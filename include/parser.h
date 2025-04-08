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
	int					input_file;
	int					output_file;
	char				*limiter;
	t_operator			operator;
	struct s_command	*next;
}						t_command;

typedef struct s_bonus
{
	t_operator			type;
	t_command			*cmd;
	struct s_bonus		*left;
	struct s_bonus		*right;
}						t_bonus;

// guardamos comandos (["echo", "hola"])
// path del comando
// archivo entrada por si hay "<"
// archivo salida por si hay ">"
// 1 si es ">>", 0 si es ">"
// para encadenar comandos cuando hay un pipe "|| &&
//" falta ver si es un file el siguente comando ejemplo "cat -e file"

t_command				*init_command(void);
void					handle_redirect(t_command *cmd, t_token **tokens);
void					print_command_list(t_command *cmds);
t_operator				resolve_operator(char *operator);
char					*get_path(char *cmd, char **env);
void					print_bonus_tree(t_bonus *node, int level);
t_bonus					*parse_tokens(t_token *tokens, char **envp);
t_bonus					*init_bonus(t_operator type, t_command *cmd,
							t_bonus *left, t_bonus *right);

const char				*operator_to_str(t_operator op);
#endif