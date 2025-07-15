#include "../../include/minishell.h"

// Lista de operadores reconocidos
int	is_operator(const char *s)
{
	return (ft_strncmp(s, "&&", 2) == 0 || ft_strncmp(s, "||", 2) == 0
		|| ft_strncmp(s, ">>", 2) == 0 || ft_strncmp(s, "<<", 2) == 0
		|| *s == '|' || *s == '<' || *s == '>' || *s == '(' || *s == ')');
}

// Extrae un operador del input
int	read_operator(const char *s, char **out)
{
	if (ft_strncmp(s, "&&", 2) == 0 || ft_strncmp(s, "||", 2) == 0
		|| ft_strncmp(s, ">>", 2) == 0 || ft_strncmp(s, "<<", 2) == 0)
	{
		*out = ft_substr(s, 0, 2);
		return (2);
	}
	else
	{
		*out = ft_substr(s, 0, 1);
		return (1);
	}
}

// Extrae una palabra, incluyendo comillas
int	read_word(const char *s, char **out)
{
	int i;
	char quote;

	i = 0;
	quote = 0;
	while (s[i] && (quote || (!ft_isspace(s[i]) && !is_operator(s + i))))
	{
		if ((s[i] == '\'' || s[i] == '"') && !quote)
			quote = s[i];
		else if (s[i] == quote)
			quote = 0;
		i++;
	}
	*out = ft_substr(s, 0, i);
	return (i);
}