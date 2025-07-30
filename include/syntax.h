/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 15:14:48 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/22 15:14:52 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef SYNTAX_H
# define SYNTAX_H

// -- Estructuras de Datos del Parser -- //
typedef struct s_token		t_token;
typedef struct s_ast		t_ast;
typedef struct s_command	t_command;
typedef struct s_redir		t_redir;

// -- Tipos de Token y Nodo -- //
enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_OPEN_PAREN,
	TOKEN_CLOSE_PAREN,
	TOKEN_BG,
	TOKEN_INVALID
};

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	NODE_BG,
	NODE_SUBSHELL
}	t_node_type;

// -- Estructuras -- //
struct s_token
{
	char			*value;
	int				type;
	char			quote_type;
	struct s_token	*next;
};

struct s_redir
{
	int		input_file;
	int		input_ord;
	int		output_file;
	int		output_ord;
	char	**limiter;
	int		heredoc_count;
	int		*heredoc_fds;
	char	**in_file;
	char	**out_file;
};

struct s_command
{
	char	**args;
	char	*path;
	t_redir	*redir;
};

struct s_ast
{
	t_node_type		type;
	t_command		*cmd;
	struct s_ast	*left;
	struct s_ast	*right;
};

// --- Prototipos de Funciones ---

// -- Lexer (ft_lexer.c y ft_lexer_2.c) --
t_token		*lexer(char *str);
t_token		*init_lexer(char *token);
void		add_back(t_token **tokens, t_token *token);
void		shift_empty_tokens(t_token **head);
char		*read_until_balanced(char *initial_line);

// -- Utilidades del Lexer (ft_lexer_utils.c) --
int			is_operator(const char *s);
int			read_operator(const char *s, char **out);
int			read_word(const char *s, char **out);
void		next_token(t_token **tokens);
int			set_token_type(char *value);

// -- Parser (ft_syntax_*.c) --
t_ast		*parse_expression(t_token **tokens, char **envp);
t_ast		*parse_pipeline(t_token **tokens, char **envp);
t_ast		*parse_factor(t_token **tokens, char **envp);
t_ast		*parse_simple_command(t_token **tokens, char **envp);
void		ft_redirects(t_command *cmd, t_token **tokens);

// -- Inicialización de Estructuras (ft_syntax_init.c) --
t_command	*init_command(void);
t_ast		*init_ast_node(t_node_type type, t_command *cmd);

// -- Validación de Sintaxis (syntax.c, syntax2.c, syntax3.c) --
int			validate_syntax(t_token *tokens);
int			check_unclosed_quotes(char *line);
int			check_operator_positions(t_token *tokens);
int			check_redirection_args(t_token *tokens);
int			check_parentheses_balance(t_token *tokens);
int			check_invalid_tokens(t_token *tokens);
int			ft_msg_syntax(char *error, char *arg);
void		resolve_command_path(t_command *cmd, char **env);

// -- Redirecciones (redirs.c) --
void		init_redir(t_redir *redir);
void		reset_redir(t_redir *redir);

#endif