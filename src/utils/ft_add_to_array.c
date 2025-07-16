#include "../../include/minishell.h"

char	**add_to_array(char **array, const char *value)
{
	int		len;
	char	**new_array;
	int		i;

	len = 0;
	if (!value)
		return (array);
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
	new_array[len] = strdup(value);
	if (!new_array[len])
	{
		free(new_array);
		return (NULL);
	}
	new_array[len + 1] = NULL;
	if (array)
		free(array);
	return (new_array);
}
