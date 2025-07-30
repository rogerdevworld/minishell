/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:56:13 by rmarrero          #+#    #+#             */
/*   Updated: 2025/03/27 11:57:56 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef UTILS_H
# define UTILS_H

// -- Wildcard Data Structure --
typedef struct s_wildcard_data
{
	DIR				*dir;
	struct dirent	*entry;
	char			*path;
	const char		*pattern;
	int				match_found;
}					t_wildcard_data;

// -- parser and ft_export -- //

char				*get_path(char *cmd, char **env);

// -- lexer -- //

// -- builtings -- //
void				ft_shlvl(t_myenv *myenv);

// -- utils -- //
void				validate_multiple_expansions(t_token *tokens, int status,
						char *line);
void				expand_before_executor(t_token **tokens, t_env *env,
						int status);
char				*ft_expand_arg_ini(char *arg, t_env *env, int s);
void				shift_empty_tokens(t_token **head);
void				shift_empty_args(char **args);

// -- free -- //
void				free_split(char **split);
void				free_minishell(t_minishell *minishell);
void				free_tokens(t_token *tokens);
void				free_command(t_command *cmd);
void				free_ast(t_ast *node);
void				free_myenv(t_myenv *myenv);
void				free_env_list(t_env *env);
void				free_env_array(char **env_array);
void				free_redir(t_redir *redir);
void				ft_clean(void *ptr);

// -- test -- //
void				print_tokens(t_token *tokens);
void				print_command(t_command *cmd);
void				print_ast(t_ast *node, int depth);

void				ft_myenv_free(t_myenv *myenv);
void				ft_destroyer(t_minishell *minishell);

/**
 * Utils used for ft_env.c
 */
void				ft_sort_export(t_env *env);
char				**ft_dup_env(char **envp);
void				ft_add_env(t_myenv *myenv, const char *key,
						const char *value);
int					env_has_key(t_env *env_list, const char *key);

/**
 * Utils used to handle different cases with quotes
 */
char				**ft_split_quotes(char const *s, char c);
char				*remove_quotes(char *str);
char				*remove_all_quotes(char *str);
char				**remove_quotes_from_args(char **args);

/**
 * Utils used for signals
 */
void				set_defaul_signals(void);
void				signals_heredoc(void);

/**
 * Utils utilizadas en los builtins
 */
/** ft_echo */
char				*ft_strjoin_free(char *s1, char *s2);
char				*ft_echo_expand(char *str, t_env *env);
int					is_valid_n_flag(char *str);
char				*copy_plain_text(char *arg, int *i);
char				*copy_single_quoted_text(const char *arg, int *i);
int					print_echo_args(t_minishell *minishsell, char **args,
						int start, int s);
char				*join_and_free(char *result, char *part);
char				*process_segment(t_minishell *minishell, char *arg, int *i,
						int s);
char				*handle_inside_double_quotes(t_minishell *minishell,
						const char *arg, int *i, int s);
char				*get_variable_value(t_minishell *minishell, char *var_name);
char				*extract_variable_name(const char *arg, int *i);

/**
 * ft_export
 */
int					env_list_size(t_env *env);
t_env				**env_to_array(t_env *env);
void				sort_env_array(t_env **array);
char				*ft_strjoin_free_env(char *s1, char *s2);
void				print_export(t_env *env);
int					count_env_items(t_env *list_env);
int					parse_export_arg(char *arg, char **key, char **value,
						int *has_equal);
void				print_invalid_identifier_error(char *arg);
void				update_or_add_env(t_env **env_list, char *key, char *value,
						int has_equal);

/**
 * Utils used for redirections, heredocs, and appends
 */
int					ft_open(char *file, int mode);
char				**add_to_array(char **array, char *value);
char				**add_to_array_heredoc(char **array, char *value);

/**
 * Utils used for expansor
 */
int					check_expansion(char *str);
char				*expand_variable_ini(const char *arg, int *i, t_env *env,
						int s);
int					check_multiple_expansions_at_start(t_token *tokens,
						t_env *env);
char				*copy_double_quoted_text_ini(const char *arg, int *i,
						t_env *env, int s);
int					is_non_empty_expansion(char *value, t_env *env);
char				*get_next_part(char *arg, int *i, t_env *env, int s);
int					append_part(char **result, char *part);
char				*handle_backslash(const char *arg, int *i);
char				*handle_plain_segment(const char *arg, int *i);
char				*handle_question_mark(int *i, int s);
char				*extract_braced_var(const char *arg, int *i);
char				*extract_simple_var(const char *arg, int *i);

int					has_internal_whitespace(const char *str);
int					set_token_type(char *value);

// -- Wildcard Functions --
void				ft_wildcards(char ***args);
char				*expand_redir_wildcard(char *pattern);
void				expand_single_wildcard(char *arg, char ***new_args,
						int *count);
int					match_pattern(const char *pattern, const char *str);
char				*extract_path(const char *pattern);
const char			*extract_pattern(const char *pattern);
char				**add_arg(char **arr, int *count, char *value);
void				find_matches_in_dir(t_wildcard_data *data, char ***args,
						int *count);

#endif