#include "../../include/minishell.h"

char	**add_to_array(char **array, char *value)
{
	int		len;
	char	**new_array;
	int		i;
	char	*clean_value;

	if (!value)
		return (array);
	clean_value = remove_all_quotes(value);
	if (!clean_value)
		return (array);
	len = 0;
	while (array && array[len])
		len++;
	new_array = malloc(sizeof(char *) * (len + 2));
	if (!new_array)
	{
		free(clean_value);
		return (NULL);
	}
	i = 0;
	while (i < len)
	{
		new_array[i] = array[i];
		i++;
	}
	new_array[len] = clean_value;
	new_array[len + 1] = NULL;
	if (array)
		free(array);
	return (new_array);
}

char	**add_to_array_heredoc(char **array, char *value)
{
	int		len;
	char	**new_array;
	int		i;

	if (!value)
		return (array);

	len = 0;
	while (array && array[len])
		len++;
	new_array = malloc(sizeof(char *) * (len + 2));
	if (!new_array)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new_array[i] = array[i];
		i++;
	}
	new_array[len] = value;
	new_array[len + 1] = NULL;
	if (array)
		free(array);
	return (new_array);
}
