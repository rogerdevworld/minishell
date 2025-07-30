# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/16 12:58:52 by rmarrero          #+#    #+#              #
#    Updated: 2025/07/29 18:13:37 by arhea            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #
# --- Mandatory --- #
NAME = minishell
SRC_DIR = ./src/mandatory/
SRC_DIR_BUI = ./src/mandatory/builtins/
SRC_DIR_LEX = ./src/mandatory/lexer/
OBJ_DIR = ./obj

SRCS = $(BSRC_DIR)minishell.c $(BSRC_DIR)main_loop.c $(BSRC_DIR)loop_processing.c $(BSRC_DIR)loop_execution.c $(BSRC_DIR)expansor.c  \
        $(BSRC_DIR)syntax/ft_lexer.c $(BSRC_DIR)syntax/ft_lexer_2.c $(BSRC_DIR)syntax/ft_lexer_utils.c $(BSRC_DIR)syntax/syntax.c $(BSRC_DIR)syntax/syntax2.c  $(BSRC_DIR)syntax/syntax3.c $(BSRC_DIR)syntax/redirs.c \
        $(BSRC_DIR)syntax/ft_syntax_redir.c $(BSRC_DIR)syntax/ft_syntax_logic.c $(BSRC_DIR)syntax/ft_syntax_init.c $(BSRC_DIR)syntax/ft_syntax_subshell.c $(BSRC_DIR)syntax/ft_syntax_pipes.c $(BSRC_DIR)syntax/ft_syntax_cmd.c \
        $(BSRC_DIR)builtins/ft_builtins.c $(BSRC_DIR)builtins/ft_pwd.c $(BSRC_DIR)builtins/ft_cd.c $(BSRC_DIR)builtins/ft_exit_builtin.c $(BSRC_DIR)builtins/ft_unset.c $(BSRC_DIR)builtins/ft_echo.c $(BSRC_DIR)builtins/ft_export.c $(BSRC_DIR)builtins/ft_env.c \
        $(BSRC_DIR)signals.c \
        $(BSRC_DIR)wildcards.c \
        $(BSRC_DIR)utils/wildcards/wildcards_utils.c \
        $(BSRC_DIR)utils/wildcards/wildcards_expand.c \
        $(BSRC_DIR)utils/wildcards/wildcards_redir.c \
        $(BSRC_DIR)utils/ft_shlvl.c $(BSRC_DIR)utils/get_path.c \
        $(BSRC_DIR)utils/cleanup/ft_free.c $(BSRC_DIR)utils/cleanup/ft_free2.c $(BSRC_DIR)utils/cleanup/ft_free3.c $(BSRC_DIR)utils/cleanup/free_split.c \
        $(BSRC_DIR)utils/builtins/ft_echo_utils.c $(BSRC_DIR)utils/builtins/ft_echo_utils2.c $(BSRC_DIR)utils/builtins/ft_echo_utils3.c \
        $(BSRC_DIR)utils/export/ft_export_utils.c $(BSRC_DIR)utils/export/ft_export_utils2.c \
        $(BSRC_DIR)utils/redirs/ft_add_to_array.c $(BSRC_DIR)utils/redirs/ft_open.c $(BSRC_DIR)utils/redirs/utils.c \
        $(BSRC_DIR)utils/signals/signals_utils.c \
        $(BSRC_DIR)utils/env/env.c $(BSRC_DIR)utils/env/env_utils.c \
        $(BSRC_DIR)utils/quotes/ft_quotes_utils.c  $(BSRC_DIR)utils/quotes/ft_split_quotes.c\
        $(BSRC_DIR)utils/expansor/expansor_utils.c $(BSRC_DIR)utils/expansor/expansor_utils2.c \
        $(BSRC_DIR)test/print_lexer.c $(BSRC_DIR)test/print_commands.c $(BSRC_DIR)test/print_ast.c $(BSRC_DIR)test/ft_print_args.c \
        $(BSRC_DIR)exec/ft_exec.c $(BSRC_DIR)exec/ft_pipe.c $(BSRC_DIR)exec/ft_here_doc.c \
        $(BSRC_DIR)exec/ft_here_doc_utils.c $(BSRC_DIR)exec/ft_here_doc_expand.c \
        $(BSRC_DIR)exec/ft_subshell.c $(BSRC_DIR)exec/ft_and.c $(BSRC_DIR)exec/ft_or.c $(BSRC_DIR)exec/ft_cmd.c $(BSRC_DIR)exec/ft_in.c $(BSRC_DIR)exec/ft_out.c	

OBJS = $(patsubst $(SRC_DIR)%.c,$(OBJ_DIR)/%.o,$(SRCS))

# --- bonus --- #
B_NAME = minishell_bonus
BSRC_DIR = ./src/bonus/
B_OBJ_DIR = ./obj_bonus

BSRCS = $(BSRC_DIR)minishell.c $(BSRC_DIR)main_loop.c $(BSRC_DIR)loop_processing.c $(BSRC_DIR)loop_execution.c $(BSRC_DIR)expansor.c  \
        $(BSRC_DIR)syntax/ft_lexer.c $(BSRC_DIR)syntax/ft_lexer_2.c $(BSRC_DIR)syntax/ft_lexer_utils.c $(BSRC_DIR)syntax/syntax.c $(BSRC_DIR)syntax/syntax2.c  $(BSRC_DIR)syntax/syntax3.c $(BSRC_DIR)syntax/redirs.c \
        $(BSRC_DIR)syntax/ft_syntax_redir.c $(BSRC_DIR)syntax/ft_syntax_logic.c $(BSRC_DIR)syntax/ft_syntax_init.c $(BSRC_DIR)syntax/ft_syntax_subshell.c $(BSRC_DIR)syntax/ft_syntax_pipes.c $(BSRC_DIR)syntax/ft_syntax_cmd.c \
        $(BSRC_DIR)builtins/ft_builtins.c $(BSRC_DIR)builtins/ft_pwd.c $(BSRC_DIR)builtins/ft_cd.c $(BSRC_DIR)builtins/ft_exit_builtin.c $(BSRC_DIR)builtins/ft_unset.c $(BSRC_DIR)builtins/ft_echo.c $(BSRC_DIR)builtins/ft_export.c $(BSRC_DIR)builtins/ft_env.c \
        $(BSRC_DIR)signals.c \
        $(BSRC_DIR)wildcards.c \
        $(BSRC_DIR)utils/wildcards/wildcards_utils.c \
        $(BSRC_DIR)utils/wildcards/wildcards_expand.c \
        $(BSRC_DIR)utils/wildcards/wildcards_redir.c \
        $(BSRC_DIR)utils/ft_shlvl.c $(BSRC_DIR)utils/get_path.c \
        $(BSRC_DIR)utils/cleanup/ft_free.c $(BSRC_DIR)utils/cleanup/ft_free2.c $(BSRC_DIR)utils/cleanup/ft_free3.c $(BSRC_DIR)utils/cleanup/free_split.c \
        $(BSRC_DIR)utils/builtins/ft_echo_utils.c $(BSRC_DIR)utils/builtins/ft_echo_utils2.c $(BSRC_DIR)utils/builtins/ft_echo_utils3.c \
        $(BSRC_DIR)utils/export/ft_export_utils.c $(BSRC_DIR)utils/export/ft_export_utils2.c \
        $(BSRC_DIR)utils/redirs/ft_add_to_array.c $(BSRC_DIR)utils/redirs/ft_open.c $(BSRC_DIR)utils/redirs/utils.c \
        $(BSRC_DIR)utils/signals/signals_utils.c \
        $(BSRC_DIR)utils/env/env.c $(BSRC_DIR)utils/env/env_utils.c \
        $(BSRC_DIR)utils/quotes/ft_quotes_utils.c  $(BSRC_DIR)utils/quotes/ft_split_quotes.c\
        $(BSRC_DIR)utils/expansor/expansor_utils.c $(BSRC_DIR)utils/expansor/expansor_utils2.c \
        $(BSRC_DIR)test/print_lexer.c $(BSRC_DIR)test/print_commands.c $(BSRC_DIR)test/print_ast.c $(BSRC_DIR)test/ft_print_args.c \
        $(BSRC_DIR)exec/ft_exec.c $(BSRC_DIR)exec/ft_pipe.c $(BSRC_DIR)exec/ft_here_doc.c \
        $(BSRC_DIR)exec/ft_here_doc_utils.c $(BSRC_DIR)exec/ft_here_doc_expand.c \
        $(BSRC_DIR)exec/ft_subshell.c $(BSRC_DIR)exec/ft_and.c $(BSRC_DIR)exec/ft_or.c $(BSRC_DIR)exec/ft_cmd.c $(BSRC_DIR)exec/ft_in.c $(BSRC_DIR)exec/ft_out.c	

# This will add _bonus to the object filenames
BOBJS = $(patsubst $(BSRC_DIR)%.c,$(B_OBJ_DIR)/%_bonus.o,$(BSRCS))

CC = cc -g -fsanitize=address,leak

CFLAGS = -I./include -Wall -Werror -Wextra -I./include
RM = rm -rf

# --- libft --- #
LIBFT = ./src/libft
EX_LIB = $(LIBFT)/libft.a
B_LIBFT = ./src/libft
B_EX_LIB = $(B_LIBFT)/libft.a

# Conditional compilation for bonus or mandatory
ifdef BONUS
    TARGET_NAME = $(B_NAME)
    OBJECTS = $(BOBJS)
    HEADER = ./include/minishell.h
    LIB = $(B_EX_LIB)
    CURRENT_SRC_DIR = $(BSRC_DIR)
    CURRENT_OBJ_DIR = $(B_OBJ_DIR)
else
    TARGET_NAME = $(NAME)
    OBJECTS = $(OBJS)
    HEADER = ./include/minishell.h
    LIB = $(EX_LIB)
    CURRENT_SRC_DIR = $(SRC_DIR)
    CURRENT_OBJ_DIR = $(OBJ_DIR)
endif

RED     = \033[31m
GREEN   = \033[32m
YELLOW  = \033[33m
BLUE    = \033[34m
RESET   = \033[0m

all: libs $(TARGET_NAME)

libs:
	@echo "$(GREEN)Compilando libft...$(RESET)"
ifdef BONUS
	@make -C $(B_LIBFT)
else
	@make -C $(LIBFT)
endif

$(CURRENT_OBJ_DIR):
	mkdir -p $(CURRENT_OBJ_DIR)
	mkdir -p $(CURRENT_OBJ_DIR)/builtins
	mkdir -p $(CURRENT_OBJ_DIR)/exec
	mkdir -p $(CURRENT_OBJ_DIR)/syntax
	mkdir -p $(CURRENT_OBJ_DIR)/utils
	mkdir -p $(CURRENT_OBJ_DIR)/utils/cleanup
	mkdir -p $(CURRENT_OBJ_DIR)/utils/builtins
	mkdir -p $(CURRENT_OBJ_DIR)/utils/export
	mkdir -p $(CURRENT_OBJ_DIR)/utils/redirs
	mkdir -p $(CURRENT_OBJ_DIR)/utils/env
	mkdir -p $(CURRENT_OBJ_DIR)/utils/signals
	mkdir -p $(CURRENT_OBJ_DIR)/utils/quotes
	mkdir -p $(CURRENT_OBJ_DIR)/utils/expansor
	mkdir -p $(CURRENT_OBJ_DIR)/utils/wildcards
	mkdir -p $(CURRENT_OBJ_DIR)/test

# Rule for compiling mandatory objects
$(OBJ_DIR)/%.o: $(SRC_DIR)%.c $(HEADER) Makefile | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule for compiling bonus objects with _bonus suffix
$(B_OBJ_DIR)/%_bonus.o: $(BSRC_DIR)%.c $(HEADER) Makefile | $(B_OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET_NAME): $(LIB) $(OBJECTS) $(HEADER)
	@echo "$(GREEN)Compilando $(TARGET_NAME)...$(RESET)"
	$(CC) $(CFLAGS) $(OBJECTS) $(LIB) -o $(TARGET_NAME) -lreadline -g
	@echo "                                                  "
	@echo "                                                  "
	@echo "# **************************************************************************** #"
	@echo "#                                                                              #"
	@echo "#                                                         :::      ::::::::    #"
	@echo "#    Minishell                                          :+:      :+:    :+:    #"
	@echo "#                                                     +:+ +:+         +:+      #"
	@echo "#    By: rmarrero rmarrero@student.42barcelona.com  +#+  +:+       +#+         #"
	@echo "#                                                 +#+#+#+#+#+   +#+            #"
	@echo "#    Created: 2024/09/16 12:58:52 by rmarrero          #+#    #+#              #"
	@echo "#    Updated: 2025/07/29 13:04:23 by rmarrero         ###   ########.fr        #"
	@echo "#                                                                              #"
	@echo "# **************************************************************************** #"
	@echo "                                                                                "
	@echo "                                                                                "
	@echo "                                                                                "
	@echo "                                                                                "
	@echo "                                   %%%%%%%%%%                                   "
	@echo "                             +%%%%%%%%%%%%%%%%%%%%+                             "
	@echo "                          %%%%%%%%%%%%%%%%%%%%%%%%%%%%                          "
	@echo "                        %%%%%%%%%......%%......%%%%%%%%%                        "
	@echo "                      %%%%%%:..%%......%%......%%..:%%%%%%                      "
	@echo "                    %%%%%%......%......%%......%......%%%%%%                    "
	@echo "                   %%%%%%%......%%.....%%.....%%......%%%%%%%                   "
	@echo "                  %%%%%..%%......%.....%%.....%......%%..%%%%%                  "
	@echo "                 %%%%%....%%.....%*....##....*%.....%%....%%%%%                 "
	@echo "                =%%%%......%*.....%....##....%.....*%......%%%%=                "
	@echo "                %%%%%%......%.....%....--....%.....%......%%%%%%                "
	@echo "                %%%%.%%:.....%.....%........%.....%......%%.%%%%                "
	@echo "               %%%%%...%%.....%....%........%....%.....%%...%%%%%               "
	@echo "               %%%%#.....%:....%................%.....%.....#%%%%               "
	@echo "               %%%%%.......%....%...%......%...%....%.......%%%%%               "
	@echo "                %%%%%=......=....%............%....=......=%%%%%                "
	@echo "                 %%%%%%%......%...=..:....:..=...%......%%%%%%%                 "
	@echo "                   #%%%%%%#.....-..............-.....#%%%%%%#                   "
	@echo "                      %%%%%%........................%%%%%%                      "
	@echo "                        %%%%........................%%%%.                       "
	@echo "                        %%%%........................%%%%                        "
	@echo "                        %%%%........................%%%%                        "
	@echo "                        %%%%%%%%%%%%%%%++%%%%%%%%%%%%%%%                        "
	@echo "                        #%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%#                        "
	@echo "                                      %%%%                                      "
	@echo "                                                                                "
	@echo "                     You are beyond the $(TARGET_NAME)                             "

	@echo "$(RESET)"

bonus:
	@echo "$(GREEN)Compilando bonus...$(RESET)"
	@$(MAKE) BONUS=1 all --no-print-directory

clean:
	@echo "$(GREEN)Eliminando archivos objeto...$(RESET)"
	$(RM) $(OBJ_DIR) $(B_OBJ_DIR)
	@make clean -C $(LIBFT)
	@make clean -C $(B_LIBFT) 2>/dev/null || true # Ignore error if bonus libft not made

fclean: clean
	@echo "$(GREEN)Eliminando ejecutable y librerías...$(RESET)"
	$(RM) -f $(NAME) $(B_NAME)
	@make fclean -C $(LIBFT)
	@make fclean -C $(B_LIBFT) 2>/dev/null || true # Ignore error if bonus libft not made

re: fclean all

.PHONY: all clean fclean re libs bonus