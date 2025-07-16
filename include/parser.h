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
}							t_node_type;

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

t_command					*init_command(void);
void						ft_redirect(t_command *cmd, t_token **tokens);
void						ft_syntax_check(t_minishell *minishell);
t_ast						*parse_expression(t_token **tokens, char **envp);

// -- redirs -- //
void						init_redir(t_redir *redir);
void						free_redir(t_redir *redir);
void						reset_redir(t_redir *redir);

#endif
