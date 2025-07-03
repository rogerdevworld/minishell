/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:56:13 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/03 14:35:34 by rmarrero         ###   ########.fr       */
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
int		execute_builtin(int cmd, char **args, char **envp, t_myenv *myenv);

// -- local commands -- //
int	pwd(char **envp);
void	clear(void);
int		ft_cd(char *path, char **envp);
void	ft_exit_builtin(void);
void	ft_echo(char **args, t_env *env);
void	ft_export(char **args, t_myenv *myenv);

void	print_export(t_env *env);
int		env_list_size(t_env *env);
t_env	**env_to_array(t_env *env);
void	sort_env_array(t_env **array);
void	export_add_or_update(t_env **env_list, char *arg);
t_env	*find_env_var(t_env *env, const char *key);
int		is_valid_identifier(const char *str);
void	ft_unset(char **args, t_myenv *myenv);
void	put_error(char *prefix, char *cmd, char *msg);
void	update_env_array(t_myenv *myenv);
char	*ft_echo_expand(char *str, t_env *env);

// -- error msg -- //
int	msg(char *error, char *arg);

#endif
