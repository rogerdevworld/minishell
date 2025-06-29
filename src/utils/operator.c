#include "../../include/minishell.h"

t_operator	resolve_operator(char *operator)
{
	if (ft_strncmp(operator, "||", 2) == 0)
		return (OR);
	if (ft_strcmp(operator, "&&") == 0)
		return (AND);
	if (ft_strcmp(operator, "|") == 0)
		return (PIPE);
	return (COMMAND);
}
const char	*operator_to_str(t_operator op)
{
	if (op == PIPE)
		return ("|");
	else if (op == AND)
		return ("&&");
	else if (op == OR)
		return ("||");
	else if (op == COMMAND)
		return ("COMMAND");
	else
		return ("UNKNOWN");
}