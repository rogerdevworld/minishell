/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_syntax_logic.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 20:58:00 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/16 20:58:07 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../include/minishell.h"

t_ast	*parse_expression(t_token **tokens, char **envp)
{
	t_ast		*left;
	t_ast		*new;
	t_node_type	type;

	left = parse_pipeline(tokens, envp);
	// --- INICIO DE LA CORRECCIÓN ---
	while (*tokens && ((*tokens)->type == TOKEN_AND
			|| (*tokens)->type == TOKEN_OR
			|| (*tokens)->type == TOKEN_BG)) // <-- Añade TOKEN_BG
	{
		if ((*tokens)->type == TOKEN_AND)
			type = NODE_AND;
		else if ((*tokens)->type == TOKEN_OR)
			type = NODE_OR;
		else
			type = NODE_BG; // <-- Añade este else
		// --- FIN DE LA CORRECCIÓN ---
		next_token(tokens);
		new = init_ast_node(type, NULL);
		new->left = left;
		new->right = parse_pipeline(tokens, envp);
		left = new;
	}
	return (left);
}

int	execute_bg(t_ast *node, t_myenv *myenv, t_minishell *minishell)
{
	pid_t	pid;
	int		status;

	g_signal = S_CMD;
	pid = fork();
	status = 0;
	if (pid == 0)
	{
		// Proceso hijo (trabajo en segundo plano)
		set_defaul_signals();
		// Desvincular de la entrada estándar del terminal
		close(STDIN_FILENO);
		open("/dev/null", O_RDONLY);
		// Ejecutar el comando de la izquierda del '&'
		exit(execute_ast(node->left, myenv, minishell, status));
	}
	else if (pid > 0)
	{
		// Proceso padre
		// NO esperar al proceso hijo (waitpid).
		// Ejecutar inmediatamente el comando de la derecha (si existe).
		if (node->right)
			status = execute_ast(node->right, myenv, minishell, status);
	}
	else
	{
		perror("fork");
		status = 1;
	}
	return (status);
}