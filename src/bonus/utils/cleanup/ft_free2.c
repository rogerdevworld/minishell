/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 13:35:52 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/15 13:35:55 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../../include/minishell.h"

void	free_env_array(char **env_array)
{
	int	i;

	i = 0;
	if (!env_array)
		return ;
	while (env_array[i])
		free(env_array[i++]);
	free(env_array);
}

void	free_minishell(t_minishell *minishell)
{
	if (!minishell)
		return ;
	if (minishell->tokens)
		free_tokens(minishell->tokens);
	minishell->tokens = NULL;
	if (minishell->ast)
		free_ast(minishell->ast);
	minishell->ast = NULL;
	if (minishell->full_line)
		free(minishell->full_line);
	// NO liberar myenv aquí, porque es una estructura persistente
	// que vive durante toda la ejecución del programa.
	// if (minishell->myenv)
	// 	free_myenv(minishell->myenv);
	free(minishell);
	minishell = NULL;
}

void	ft_free_env_array(char **env)
{
	int	i;

	i = 0;
	if (!env)
		return ;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

void	ft_env_free(t_env *env_list)
{
	t_env	*tmp;

	while (env_list)
	{
		tmp = env_list->next;
		free(env_list->key);
		if (env_list->content)
			free(env_list->content);
		free(env_list);
		env_list = tmp;
	}
}

// -- free for env's -- //
void	ft_myenv_free(t_myenv *myenv)
{
	if (!myenv)
		return ;
	ft_free_env_array(myenv->env);
	ft_env_free(myenv->list_env);
	free(myenv);
}

void	ft_clean(void *ptr)
{
	if (!ptr)
		return ;
	free(ptr);
	ptr = NULL;
}
