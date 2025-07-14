/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:56:13 by rmarrero          #+#    #+#             */
/*   Updated: 2025/04/09 22:22:59 by xviladri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef ENV_H
#define ENV_H

// -- ROGER X 2!! se puede crear una variable o funcion global (por ej. get_myenv()), para poder acceder a t_myenv?? Y asi se la pasamos a la funcion ft_export(args, myenv) --> ft_export(args, get_myenv()); Y asi no tener que pasar tantos argumentos a las funciones de builtins.c --//

typedef struct s_env
{
	char *key;
	char *content;
	struct s_env *next;
} t_env;

typedef struct s_myenv
{
	char **env;
	t_env *list_env;
} t_myenv;

t_env *ft_env_new(char *key, char *content);
void ft_env_add_back(t_env **lst, t_env *new);
void ft_env(t_env **env_list, char **envp);
t_myenv *ft_myenv(char **envp);
void free_myenv(t_myenv *myenv);
void free_env_list(t_env *env);
void free_env_array(char **env_array);

void print_env(t_myenv *myenv);
void ft_sort_export(t_env *env);
char **ft_dup_env(char **envp);
void ft_add_env(t_myenv *myenv, const char *key, const char *value);
int env_has_key(t_env *env_list, const char *key);

#endif
