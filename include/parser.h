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

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	NODE_SUBSHELL
}	t_node_type;

typedef struct s_command
{
	char	**args;
	char	*path;
	int		input_file;
	int		output_file;
	char	*limiter;
}	t_command;

typedef struct s_ast
{
	t_node_type		type;
	t_command		*cmd;
	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast;

typedef struct s_minishell	t_minishell;

t_command	*init_command(void);
void		handle_redirect(t_command *cmd, t_token **tokens);
char		*remove_quotes(char *str);
t_command	*parse_tokens(t_token *tokens, char **envp);
void		print_command_list(t_command *cmds);
char		*get_path(char *cmd, char **env);
char		*operator_to_str(int token_type);

void		ft_syntax_check(t_minishell *minishell);

// 🔽 Agrega estas dos declaraciones para que no haya errores en parser.c
t_ast		*parse_expression(t_token **tokens, char **envp);
void		print_ast(t_ast *node, int depth);
void print_command(t_command *cmd);

#endif
