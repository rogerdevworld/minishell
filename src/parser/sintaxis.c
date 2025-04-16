/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xviladri <xviladri@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:43:57 by xviladri          #+#    #+#             */
/*   Updated: 2025/04/01 14:11:55 by xviladri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

/*
    -- cosas para corregir en el parser (incluyendo bonus)

    1. `>&&` // operador lógico sin comando antes o después
       Explicación: No puede haber un operador lógico `&&` o `||` sin que haya un comando válido antes o después de él.

    2. `>|` // redirección mal formada, debe ser seguida de un archivo válido
       Explicación: El operador de redirección `>|` debe estar seguido de un archivo válido y no debe ser seguido por otros operadores como pipes (`|`).

    3. `'` o `"` sueltas sin par adecuado
       Explicación: Las comillas simples `'` o dobles `"` deben estar emparejadas correctamente. Las comillas sueltas son un error de sintaxis.

    4. `> |cat` // redirección seguida de un pipe sin un comando válido después
       Explicación: Una redirección `>` o `>>` seguida de un pipe (`|`) no tiene sentido. Debe ser seguida por un archivo válido o un comando.

    5. `>` o `>>` mal formados // redirección seguida de un operador o sin archivo válido
       Explicación: Las redirecciones deben ser seguidas de un archivo o un comando válido. No se deben permitir secuencias como `>|` o `>>|`.

    6. Comandos vacíos después de operadores
       Explicación: Si un operador como `&&`, `||`, `>`, o `>>` es seguido por un comando vacío o incorrecto, debe marcarse como error.

    7. Operadores al principio o al final de la línea
       Explicación: Los operadores `&&`, `||`, `>`, `>>`, y `|` no pueden estar al principio o al final de la cadena sin un comando adecuado.

    8. Espacios innecesarios entre operadores y comandos
       Explicación: Se deben eliminar los espacios innecesarios alrededor de los operadores. Por ejemplo, `>    file` debería ser interpretado correctamente.

    9. Verificación de archivos en redirección
       Explicación: Al usar redirección con `>` o `>>`, debes verificar que el archivo sea accesible y válido. No debe ser un directorio o un archivo no permitido.

   10. Múltiples pipes o redirecciones sin comandos entre ellos
       Explicación: No se deben permitir múltiples operadores como `|` o `>>` sin un comando o archivo válido entre ellos. Ejemplo de error: `| |`, `>> |`, `> |`.

   11. Redirección de error sin redirección estándar
       Explicación: Cuando se usa redirección de error (`2>`), debe haber una redirección estándar (`>`) o un archivo de destino adecuado.

   12. Comillas dentro de comillas mal interpretadas
       Explicación: Si se usan comillas dentro de comillas (por ejemplo, `'"'`), asegúrate de que el parser las interprete correctamente y no las vea como delimitadores erróneos.

   13. Verificación de comandos válidos antes de pipes
       Explicación: Asegúrate de que siempre haya un comando válido antes de un pipe (`|`). No puede haber un pipe al principio de la cadena ni después de un operador sin un comando válido.

   14. `&&` y `||` con ejecución condicional
       Explicación: El operador `&&` debe ejecutar el siguiente comando solo si el anterior tiene éxito, y `||` debe ejecutarse si el anterior falla. Asegúrate de que los comandos alrededor de estos operadores sean válidos.

   15. Ejecución en segundo plano con `&`
       Explicación: El operador `&` al final de un comando debe hacer que el comando se ejecute en segundo plano. Si el `&` está solo o seguido de espacios, debe marcarse como error.

   16. Secuencia de ejecución de operadores `&&` y `||`
       Explicación: Si hay una secuencia de operadores como `comando1 && comando2 || comando3`, asegúrate de que la ejecución de los comandos siga el orden correcto (ejecutar primero `&&`, luego `||`).

   17. Variables de entorno (export)
       Explicación: Asegúrate de que las variables de entorno sean correctamente asignadas y exportadas con el formato adecuado (`export VAR=value`).

   18. Ejecución de comandos internos (builtins)
       Explicación: Los comandos internos como `cd`, `exit`, `echo`, `pwd`, etc., deben ejecutarse internamente sin buscar un ejecutable en el PATH.
*/
