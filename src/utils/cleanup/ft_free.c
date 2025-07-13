#include "../../../include/minishell.h"

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		free(tmp->value);
		free(tmp);
	}
}

void	free_command(t_command *cmd)
{
	if (!cmd)
		return ;
	free_split(cmd->args);
	free(cmd->path);
	if (cmd->redir)
	{
		free(cmd->redir->limiter);
		free(cmd->redir->file);
		free(cmd->redir);
	}
	free(cmd);
}

void	free_ast(t_ast *node)
{
	if (!node)
		return ;
	free_command(node->cmd);
	free_ast(node->left);
	free_ast(node->right);
	free(node);
}

void	free_myenv(t_myenv *myenv)
{
	if (!myenv)
		return ;
	free_env_list(myenv->list_env);
	free_env_array(myenv->env);
	free(myenv);
}

void	free_env_list(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->key);
		free(env->content);
		free(env);
		env = tmp;
	}
}
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
	free_tokens(minishell->tokens);
	free_command(minishell->cmd);
	free_ast(minishell->ast);
	free_myenv(minishell->env);
	free(minishell);
}
