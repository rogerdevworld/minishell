/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:56:13 by rmarrero          #+#    #+#             */
/*   Updated: 2025/06/29 17:13:15 by xviladri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef BUILTINS_H
# define BUILTINS_H

// -- lexer.h -- //

// --- internal commads -- //
enum
{
	CD,
	EXIT,
	ECHO,
	PWD,
	EXPORT,
	UNSET,
	CLEAR,
	ENV
};

// -- internal to execve() -- //
void	built(t_command *cmd, char **envp);
int		get_builtin_cmd(char *cmd);
void	execute_builtin(int cmd, char **args, char **envp, t_myenv *myenv);
// -- local commands -- //
void	pwd(char **envp);
void	clear(void);
void	ft_cd(const char *path, char **envp);
void	ft_exit_builtin(void);
void	print_argument(char *arg, t_env *env);
void	ft_echo(char **args, t_env *env);
void	ft_export(char **args, t_myenv *myenv);
void	print_export(t_env *env);
int		env_list_size(t_env *env);
t_env	**env_to_array(t_env *env);
void	sort_env_array(t_env **array);
void	export_add_or_update(t_env **env_list, char *arg);
t_env	*find_env_var(t_env *env, const char *key);
int		is_valid_identifier(const char *str);
int		ft_check_wrong_char(char *s);
void	remove_env_node(t_env **env_list, const char *key);
void	ft_unset(char **args, t_myenv *myenv);
void	put_error(char *prefix, char *cmd, char *msg);
void	update_env_array(t_myenv *myenv);
char	*ft_echo_expand(char *str, t_env *env);

#endif
