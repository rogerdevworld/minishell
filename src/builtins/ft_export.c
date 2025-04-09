/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:05:47 by rmarrero          #+#    #+#             */
/*   Updated: 2025/04/01 13:05:50 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/minishell.h"

// -- ya esta funciona el env para aplciarle al unset y el export 
// nuestra propia versiond el env y la copia hay funcion para agregar un elemento
// voy a cear una para expandir las key a su valor algo como:
// echo $PATH salida my_getenv("PATH") y neustra lista deberia contener adicionales 
// export a=a seria como add a=a  en t_env con la funcion ft_env_add(key, value);
// al final de la lista de t_env
/*void	ft_export(char **args, char **envp)
{
	if (!args[1])
	{

	}
}*/
