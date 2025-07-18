#ifndef SYNTAX_H
# define SYNTAX_H

// -- lexer.h -- //
enum
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
	TOKEN_INVALID
};

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	NODE_SUBSHELL
}							t_node_type;

typedef struct s_token
{
	char					*value;
	int						type;
	char					quote_type;
	struct s_token			*next;
}							t_token;

typedef struct s_word
{
	char					*value;
	char					quote_type;
}							t_word;

typedef struct s_redir
{
	int						input_file;
	int						output_file;
	char					**limiter;
	int						heredoc_count;
	int						*heredoc_fds;
	char					**in_file;
	char					**out_file;
}							t_redir;

typedef struct s_command
{
	char					**args;
	char					*path;
	t_redir					*redir;
}							t_command;

typedef struct s_ast
{
	t_node_type				type;
	t_command				*cmd;
	struct s_ast			*left;
	struct s_ast			*right;
}							t_ast;

typedef struct s_minishell	t_minishell;

t_token						*lexer(char *str);
t_token						*init_lexer(char *token);
// t_token				*init_lexer(char *token, char quote_type);
void						add_back(t_token **tokens, t_token *token);
int							validate_syntax(t_token *tokens);

int							is_operator(const char *s);
int							read_operator(const char *s, char **out);
int							read_word(const char *s, char **out);
void						next_token(t_token **tokens);

// -- init ast & cmd -- //
t_command					*init_command(void);
t_ast						*init_ast_node(t_node_type type, t_command *cmd);

// -- parser in ast for cases -- //
void						ft_redirects(t_command *cmd, t_token **tokens);
t_ast						*parse_simple_command(t_token **tokens,
								char **envp);
t_ast						*parse_factor(t_token **tokens, char **envp);
t_ast						*parse_pipeline(t_token **tokens, char **envp);
t_ast						*parse_expression(t_token **tokens, char **envp);

// -- checker in syntax >&&  >ls && empty
void						ft_syntax_check(t_minishell *minishell);
t_ast						*parse_expression(t_token **tokens, char **envp);

// -- redirs -- //
void						init_redir(t_redir *redir);
void						free_redir(t_redir *redir);
void						reset_redir(t_redir *redir);

// -- cheker open quotes -- //
int							check_unclosed_quotes(char *line);
void						resolve_command_path(t_command *cmd, char **env);
void shift_empty_tokens(t_token **head);

#endif
