/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xviladri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:56:13 by rmarrero          #+#    #+#             */
/*   Updated: 2025/06/29 14:49:21 by xviladri         ###   ########.fr       */
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
// -- parser.c -- //
t_command				*init_command(void);
void					handle_redirect(t_command *cmd, t_token **tokens);
void					process_token(t_token **tokens, t_command *current,
							int *i, char **envp);
t_command				*parse_tokens(t_token *tokens, char **envp);
// -- get_path.c -- //
char					*remove_quotes(char *str);
char					*get_path(char *cmd, char **env);
// -- ft_open.c -- //
int						ft_open(char *file, int mode);
const char				*operator_to_str(t_operator op);
t_operator				resolve_operator(char *operator);
#endif
