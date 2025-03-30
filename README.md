# 🛠 Planificación del Proyecto Minishell

## 🔧 Organización del Proyecto

### 📂 Estructura de Archivos

```
📁 minishell/
│── 📄 Makefile
│── 📄 minishell.c        // Función principal
│── 📄 lexer.c            // Tokenización
│── 📄 parser.c           // Construcción de estructuras de comandos
│── 📄 executor.c         // Ejecución de comandos
│── 📄 builtins.c         // Implementación de comandos internos
│── 📄 signals.c          // Manejo de señales
│── 📄 env.c              // Manejo de variables de entorno
│── 📄 redirections.c     // Manejo de <, >, <<, >>
│── 📄 pipes.c            // Manejo de pipes (|)
│── 📄 heredoc.c          // Manejo del heredoc
│── 📄 wildcards.c        // Implementación del wildcard *
│── 📄 logical_operators.c // Implementación de && y ||
│── 📄 utils.c            // Funciones auxiliares
│── 📁 include/
│   └📄 minishell.h      // Header principal
│── 📁 libft/             // Librería auxiliar
```

### 🔀 Ramas de Git

- `main` → Rama principal estable.
- `dev` → Rama de desarrollo.
- `feature/lexer`, `feature/parser`, `feature/executor`, etc. → Desarrollo de features.
- `bonus/logical_operators` → Implementación de `&&` y `||`.
- `bonus/wildcards` → Implementación del `*`.

## 🚀 Desarrollo Paso a Paso

### 🛠 Paso 1: Configuración Inicial

**Responsable:** Todos

- Crear el repositorio y configurar el `Makefile`.

### 📄 Paso 2: Lexer (Tokenización)

**Responsable:** Roger

- Separar la entrada en tokens (`echo`, `ls`, `<`, `>`, `|`, etc.).

### 📝 Paso 3: Parser (Estructura de Comandos)

**Responsable:** Xenia

- Construir estructuras para comandos y operadores (`&&`, `||`).

### 🚀 Paso 4: Ejecutor (Execution)

**Responsable:** ambos

- Implementar `execve` para ejecutar comandos.

### ⚙️ Paso 5: Built-ins

**Responsable:** Roger

- Implementar `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`.

### ⚡ Paso 6: Manejo de Señales

**Responsable:** Xenia

- Implementar `CTRL+C`, `CTRL+D`, `CTRL+\`.

### 📌 Paso 7: Redirecciones y Pipes

**Responsable:** ambos

- Implementar `<`, `>`, `>>`, `|`.

### 📚 Paso 8: Heredoc

**Responsable:** Roger

- Implementar `<<` para recibir delimitadores.

### 🌐 Paso 9: Variables de Entorno

**Responsable:** Xenia

- Implementar `$VAR` y `$?`.

## 🔹 Bonus

### ✅ Bonus 1: Operadores Lógicos (`&&` y `||`)

**Responsable:** Roger

- Implementar `&&` y `||` para ejecución condicional.

### ✅ Bonus 2: Wildcards (`*`)

**Responsable:** Xenia

- Implementar `*` para expansion de archivos.

## ⏳ Ramas y Momentos de Merge

| Fase                   | Ramas que se mergean           |      |                                   |
| ---------------------- | ------------------------------ | ---- | --------------------------------- |
| Lexer                  | `feature/lexer` → `dev`        |      |                                   |
| Parser                 | `feature/parser` → `dev`       |      |                                   |
| Executor               | `feature/executor` → `dev`     |      |                                   |
| Built-ins              | `feature/builtins` → `dev`     |      |                                   |
| Señales                | `feature/signals` → `dev`      |      |                                   |
| Redirecciones          | `feature/redirections` → `dev` |      |                                   |
| Pipes                  | `feature/pipes` → `dev`        |      |                                   |
| Heredoc                | `feature/heredoc` → `dev`      |      |                                   |
| Variables de entorno   | `feature/env` → `dev`          |      |                                   |
| **Integración final**  | `dev` → `main`                 |      |                                   |
| \*\*Bonus 1: && y      |                                | \*\* | `bonus/logical_operators` → `dev` |
| **Bonus 2: Wildcards** | `bonus/wildcards` → `dev`      |      |                                   |

## 🔥 Buenas Prácticas

- Hacer commits pequeños y descriptivos.
- Usar nombres de ramas claros (`feature/parser`, `fix/memory-leak`).
- Revisar código de los compañeros antes de hacer merge.
- Mantener `main` siempre estable.