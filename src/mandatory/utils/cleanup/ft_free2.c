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

/**
 * Frees a null-terminated array of strings.
 * It iterates through each string in the array, freeing its memory,
 * and then frees the array itself.
 */
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

/**
 * Frees all dynamically allocated memory associated with the 
	main `t_minishell` structure.
 * This includes freeing the token list, the Abstract Syntax Tree (AST),
 * the full input line, and finally the `minishell` structure itself.
 * Pointers are set to NULL after freeing to prevent use-after-free errors.
 */
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
	free(minishell);
	minishell = NULL;
}

/**
 * Frees a null-terminated array of strings.
 * This function is identical in purpose to `free_env_array`,
	suggesting a possible redundancy
 * or specific naming convention within the project. It iterates 
	through each string,
	frees it,
 * and then frees the array itself.
 */
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

/**
 * Frees all nodes in a linked list of `t_env` structures.
 * It iterates through the list, deallocating memory for each node's key,
 * content (if it exists), and the node itself.
 */
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

/**
 * Frees all dynamically allocated memory within the `t_myenv` structure.
 * This involves freeing the `char** env` array and the `t_env` linked list,
 * then finally the `t_myenv` structure itself.
 */
void	ft_myenv_free(t_myenv *myenv)
{
	if (!myenv)
		return ;
	ft_free_env_array(myenv->env);
	ft_env_free(myenv->list_env);
	free(myenv);
}
