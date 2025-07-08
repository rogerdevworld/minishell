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
	char					**args;
	char					*path;
	int						input_file;
	int						output_file;
	char					*limiter;
	char						*operator;
	struct s_command		*next;
}							t_command;

typedef struct s_minishell	t_minishell;

t_command					*init_command(void);
void						handle_redirect(t_command *cmd, t_token **tokens);
char						*remove_quotes(char *str);
t_command					*parse_tokens(t_token *tokens, char **envp);
void						print_command_list(t_command *cmds);
// t_operator					resolve_operator(char *operator);
char						*get_path(char *cmd, char **env);
char						*operator_to_str(int token_type);

void						ft_syntax_check(t_minishell *minishell);

#endif
