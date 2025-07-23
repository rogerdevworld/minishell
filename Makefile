# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/16 12:58:52 by rmarrero          #+#    #+#              #
#    Updated: 2025/07/23 13:58:58 by rmarrero         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #
# --- Mandatory --- #
NAME = minishell
SRC_DIR = ./src/
SRC_DIR_BUI = ./src/builtins/
SRC_DIR_LEX = ./src/lexer/
OBJ_DIR = ./obj
OBJ_DIR_BUI = ./obj/builtins/
OBJ_DIR_LEX = ./obj/lexer/

SRCS =	$(SRC_DIR)minishell.c $(SRC_DIR)init.c $(SRC_DIR)exit.c $(SRC_DIR)expansor.c  \
		$(SRC_DIR)syntax/ft_lexer.c $(SRC_DIR)syntax/ft_lexer_utils.c $(SRC_DIR)syntax/syntax.c $(SRC_DIR)syntax/syntax2.c $(SRC_DIR)syntax/redirs.c \
		$(SRC_DIR)syntax/ft_syntax_redir.c $(SRC_DIR)syntax/ft_syntax_logic.c $(SRC_DIR)syntax/ft_syntax_init.c $(SRC_DIR)syntax/ft_syntax_subshell.c $(SRC_DIR)syntax/ft_syntax_pipes.c $(SRC_DIR)syntax/ft_syntax_cmd.c \
		$(SRC_DIR_BUI)builtins.c $(SRC_DIR_BUI)pwd.c $(SRC_DIR_BUI)clear.c $(SRC_DIR_BUI)ft_cd.c $(SRC_DIR_BUI)ft_exit_builtin.c $(SRC_DIR_BUI)ft_unset.c $(SRC_DIR_BUI)ft_echo.c $(SRC_DIR_BUI)ft_export.c $(SRC_DIR_BUI)ft_env.c  \
		$(SRC_DIR)signals.c \
		$(SRC_DIR)wildcards.c \
		$(SRC_DIR)utils/ft_shlvl.c $(SRC_DIR)utils/get_path.c \
		$(SRC_DIR)utils/cleanup/ft_free.c $(SRC_DIR)utils/cleanup/ft_free2.c $(SRC_DIR)utils/cleanup/free_split.c $(SRC_DIR)utils/cleanup/ft_destroyer.c \
		$(SRC_DIR)utils/builtins/ft_echo_utils.c $(SRC_DIR)utils/builtins/ft_echo_utils2.c $(SRC_DIR)utils/builtins/ft_echo_utils3.c \
		$(SRC_DIR)utils/export/ft_export_utils.c $(SRC_DIR)utils/export/ft_export_utils2.c \
		$(SRC_DIR)utils/redirs/ft_add_to_array.c $(SRC_DIR)utils/redirs/ft_open.c \
		$(SRC_DIR)utils/signals/signals_utils.c \
		$(SRC_DIR)utils/env/env.c $(SRC_DIR)utils/env/env_utils.c \
		$(SRC_DIR)utils/quotes/ft_quotes_utils.c  $(SRC_DIR)utils/quotes/ft_split_quotes.c\
		$(SRC_DIR)utils/expansor/expansor_utils.c $(SRC_DIR)utils/expansor/expansor_utils2.c \
		$(SRC_DIR)test/print_lexer.c $(SRC_DIR)test/print_commands.c $(SRC_DIR)test/print_ast.c $(SRC_DIR)test/ft_print_args.c \
		$(SRC_DIR)exec/ft_exec.c $(SRC_DIR)exec/ft_pipe.c $(SRC_DIR)exec/ft_here_doc.c $(SRC_DIR)exec/ft_subshell.c $(SRC_DIR)exec/ft_and.c $(SRC_DIR)exec/ft_or.c $(SRC_DIR)exec/ft_cmd.c $(SRC_DIR)exec/ft_in.c $(SRC_DIR)exec/ft_out.c

OBJS = $(SRCS:$(SRC_DIR)%.c=$(OBJ_DIR)/%.o)

# --- bonus --- #
BSRC_DIR = ./src/bonus/
OBJ_DIR = ./obj

BSRCS =	$(BSRC_DIR)pipex_bonus.c $(BSRC_DIR)parse_bonus.c $(BSRC_DIR)cmd_bonus.c $(BSRC_DIR)here_doc_bonus.c  $(BSRC_DIR)pipex_utils_bonus.c

BOBJS = $(BSRCS:$(BSRC_DIR)%.c=$(OBJ_DIR)/%.o)

CC = cc -g
CFLAGS = -I./include -Wall -Werror -Wextra -I./include -fsanitize=leak
RM = rm -rf

# --- libft --- #
LIBFT = ./src/utils/libft
EX_LIB = $(LIBFT)/libft.a

ifdef BONUS
	OBJECTS = $(BOBJS)
	HEADER = ./include/pipex_bonus.h
	SRC_DIR = ./src/bonus/
else
	OBJECTS = $(OBJS)
	HEADER = ./include/minishell.h
	SRC_DIR = ./src/
endif

RED     = \033[31m
GREEN   = \033[32m
YELLOW  = \033[33m
BLUE    = \033[34m
RESET   = \033[0m

all: libs $(NAME)

libs:
	@echo "$(GREEN)Compilando libft...$(RESET)"
	@make -C $(LIBFT)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
	mkdir -p ./obj/builtins
	mkdir -p ./obj/exec
	mkdir -p ./obj/design
	mkdir -p ./obj/syntax
	mkdir -p ./obj/utils
	mkdir -p ./obj/utils/cleanup
	mkdir -p ./obj/utils/builtins
	mkdir -p ./obj/utils/export
	mkdir -p ./obj/utils/redirs
	mkdir -p ./obj/utils/env
	mkdir -p ./obj/utils/signals
	mkdir -p ./obj/utils/quotes
	mkdir -p ./obj/utils/expansor
	mkdir -p ./obj/test

$(OBJ_DIR)/%.o: $(SRC_DIR)%.c $(HEADER) Makefile | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(EX_LIB) $(OBJECTS) $(HEADER)
	@echo "$(GREEN)Compilando $(NAME)...$(RESET)"
	$(CC) $(CFLAGS) $(OBJECTS) $(EX_LIB) -o $(NAME) -lreadline -g
	@echo "$(BLUE)"
	@echo "$(YELLOW)           ($(RESET)__$(YELLOW))\           $(RESET)"
	@echo "$(YELLOW)           ($(RESET)oo$(YELLOW))\\________  $(RESET)"
	@echo "$(RESET)           /|| \\        \\ $(NAME) ready$(RESET)"
	@echo "$(RESET)              ||------w | $(RESET)"
	@echo "$(RESET)              ||       || $(RESET)"
	@echo "$(YELLOW)THE COW MAKES MUUUUUUUUUU!$(RESET)"
	@echo "$(RESET)"

bonus:
	@echo "$(GREEN)Compilando bonus...$(RESET)"
	@$(MAKE) BONUS=42 --no-print-directory

clean:
	@echo "$(GREEN)Eliminando archivos objeto...$(RESET)"
	$(RM) $(OBJ_DIR)
	@make clean -C $(LIBFT)

fclean: clean
	@echo "$(GREEN)Eliminando ejecutable y librerías...$(RESET)"
	$(RM) -f $(NAME)
	@make fclean -C $(LIBFT)

re: fclean all

.PHONY: all clean fclean re libs
