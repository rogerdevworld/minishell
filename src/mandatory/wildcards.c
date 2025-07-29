/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 21:49:28 by rmarrero          #+#    #+#             */
/*   Updated: 2025/02/22 21:49:33 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/minishell.h"

// Compara una cadena con un patrón que puede incluir '*'
int	match_pattern(const char *pattern, const char *str)
{
	if (!*pattern)
		return (!*str);
	if (*pattern == '*')
	{
		while (*pattern == '*')
			pattern++;
		if (!*pattern)
			return (1);
		while (*str)
		{
			if (match_pattern(pattern, str))
				return (1);
			str++;
		}
		return (0);
	}
	else if (*pattern == *str)
		return (match_pattern(pattern + 1, str + 1));
	else
		return (0);
}

// Extrae la parte del directorio de un patrón (ej: "src/*.c" -> "src")
char	*extract_path(const char *pattern)
{
	char	*slash;
	size_t	len;
	char	*path;

	slash = ft_strrchr(pattern, '/');
	if (!slash)
		return (ft_strdup("."));
	len = slash - pattern;
	path = malloc(len + 1);
	if (!path)
		return (NULL);
	ft_strlcpy(path, pattern, len + 1);
	return (path);
}

// Extrae la parte del patrón de un path (ej: "src/*.c" -> "*.c")
const char	*extract_pattern(const char *pattern)
{
	char	*slash;

	slash = ft_strrchr(pattern, '/');
	if (!slash)
		return (pattern);
	return (slash + 1);
}

// Función auxiliar para añadir de forma segura un argumento a un array dinámico
static char	**add_arg(char **arr, int *count, char *value)
{
	char	**new_arr;

	// Se usa realloc para redimensionar el array de argumentos
	new_arr = realloc(arr, sizeof(char *) * (*count + 2));
	if (!new_arr)
	{
		free(value); // Liberar en caso de fallo de realloc
		return (arr);
	}
	new_arr[*count] = value;
	(*count)++;
	new_arr[*count] = NULL; // Siempre terminar con NULL
	return (new_arr);
}

// Función principal para expandir wildcards
void	ft_wildcards(char ***args)
{
	char	**new_args;
	int		new_arg_count;
	int		i;

	new_args = NULL;
	new_arg_count = 0;
	i = 0;
	if (!*args)
		return ;
	while ((*args)[i])
	{
		// CORRECCIÓN: Usar ft_strchr para detectar '*' en cualquier parte del argumento
		if (ft_strchr((*args)[i], '*'))
		{
			DIR				*dir;
			struct dirent	*entry;
			char			*path;
			const char		*pattern;
			int				match_found;

			match_found = 0;
			path = extract_path((*args)[i]);
			pattern = extract_pattern((*args)[i]);
			dir = opendir(path);
			if (dir)
			{
				while ((entry = readdir(dir)))
				{
					// Ignorar archivos ocultos (que empiezan con '.') a menos que el patrón lo pida
					if (entry->d_name[0] == '.' && pattern[0] != '.')
						continue ;
					if (match_pattern(pattern, entry->d_name))
					{
						match_found = 1;
						// No añadir "./" si estamos en el directorio actual
						if (ft_strcmp(path, ".") == 0)
							new_args = add_arg(new_args, &new_arg_count,
									ft_strdup(entry->d_name));
						else
						{
							char *tmp_path = ft_strjoin(path, "/");
							new_args = add_arg(new_args, &new_arg_count,
									ft_strjoin(tmp_path, entry->d_name));
							free(tmp_path);
						}
					}
				}
				closedir(dir);
			}
			free(path);
			// CORRECCIÓN: Si no hay coincidencias, añadir el patrón original
			if (!match_found)
				new_args = add_arg(new_args, &new_arg_count, ft_strdup((*args)[i]));
		}
		else
		{
			// Si no es un wildcard, simplemente copiar el argumento
			new_args = add_arg(new_args, &new_arg_count, ft_strdup((*args)[i]));
		}
		i++;
	}
	// Liberar el array de argumentos antiguo y reemplazarlo por el nuevo
	free_split(*args);
	*args = new_args;
}

// src/utils/wildcards_utils.c (o src/wildcards.c)

// NOTA: Esta función es una copia de la que es estática en wildcards.c,
// pero la necesitamos aquí de forma no estática.
char	**add_arg_for_redir(char **arr, int *count, char *value)
{
	char	**new_arr;

	new_arr = realloc(arr, sizeof(char *) * (*count + 2));
	if (!new_arr)
	{
		free(value);
		return (arr);
	}
	new_arr[*count] = value;
	(*count)++;
	new_arr[*count] = NULL;
	return (new_arr);
}

// Esta función expande un patrón y gestiona los errores de redirección.
char	*expand_redir_wildcard(char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	char			*path;
	const char		*p;
	char			**matches;
	int				count;

	if (!pattern || !ft_strchr(pattern, '*'))
		return (ft_strdup(pattern));
	matches = NULL;
	count = 0;
	path = extract_path(pattern);
	p = extract_pattern(pattern);
	dir = opendir(path);
	if (dir != NULL)
	{
		while ((entry = readdir(dir)))
		{
			if (entry->d_name[0] == '.' && p[0] != '.')
				continue ;
			if (match_pattern(p, entry->d_name))
			{
				char	*full_match;
				char	*tmp_path;

				if (ft_strcmp(path, ".") == 0)
					full_match = ft_strdup(entry->d_name);
				else
				{
					tmp_path = ft_strjoin(path, "/");
					full_match = ft_strjoin(tmp_path, entry->d_name);
					free(tmp_path);
				}
				matches = add_arg_for_redir(matches, &count, full_match);
			}
		}
		closedir(dir);
	}
	free(path);
	if (count > 1)
		return (msg("ambiguous redirect", pattern), free_split(matches), NULL);
	if (count == 0)
		return (msg("No such file or directory", pattern), free_split(matches), NULL);
	return (matches[0]); // Devuelve el único match (el array se liberará después)
}