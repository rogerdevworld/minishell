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

SRCS =  $(SRC_DIR)minishell.c $(SRC_DIR)init.c $(SRC_DIR)exit.c $(SRC_DIR)expansor.c  \
        $(SRC_DIR)syntax/ft_lexer.c $(SRC_DIR)syntax/ft_lexer_utils.c $(SRC_DIR)syntax/syntax.c $(SRC_DIR)syntax/syntax2.c $(SRC_DIR)syntax/redirs.c \
        $(SRC_DIR)syntax/ft_syntax_redir.c $(SRC_DIR)syntax/ft_syntax_logic.c $(SRC_DIR)syntax/ft_syntax_init.c $(SRC_DIR)syntax/ft_syntax_subshell.c $(SRC_DIR)syntax/ft_syntax_pipes.c $(SRC_DIR)syntax/ft_syntax_cmd.c \
        $(SRC_DIR_BUI)builtins.c $(SRC_DIR_BUI)ft_pwd.c $(SRC_DIR_BUI)ft_cd.c $(SRC_DIR_BUI)ft_exit_builtin.c $(SRC_DIR_BUI)ft_unset.c $(SRC_DIR_BUI)ft_echo.c $(SRC_DIR_BUI)ft_export.c $(SRC_DIR_BUI)ft_env.c \
        $(SRC_DIR)signals.c \
        $(SRC_DIR)wildcards.c \
        $(SRC_DIR)utils/ft_shlvl.c $(SRC_DIR)utils/get_path.c \
        $(SRC_DIR)utils/cleanup/ft_free.c $(SRC_DIR)utils/cleanup/ft_free2.c $(SRC_DIR)utils/cleanup/free_split.c $(SRC_DIR)utils/cleanup/ft_destroyer.c \
        $(SRC_DIR)utils/builtins/ft_echo_utils.c $(SRC_DIR)utils/builtins/ft_echo_utils2.c $(SRC_DIR)utils/builtins/ft_echo_utils3.c \
        $(SRC_DIR)utils/export/ft_export_utils.c $(SRC_DIR)utils/export/ft_export_utils2.c \
        $(SRC_DIR)utils/redirs/ft_add_to_array.c $(SRC_DIR)utils/redirs/ft_open.c $(SRC_DIR)utils/redirs/utils.c \
        $(SRC_DIR)utils/signals/signals_utils.c \
        $(SRC_DIR)utils/env/env.c $(SRC_DIR)utils/env/env_utils.c \
        $(SRC_DIR)utils/quotes/ft_quotes_utils.c  $(SRC_DIR)utils/quotes/ft_split_quotes.c\
        $(SRC_DIR)utils/expansor/expansor_utils.c $(SRC_DIR)utils/expansor/expansor_utils2.c \
        $(SRC_DIR)test/print_lexer.c $(SRC_DIR)test/print_commands.c $(SRC_DIR)test/print_ast.c $(SRC_DIR)test/ft_print_args.c \
        $(SRC_DIR)exec/ft_exec.c $(SRC_DIR)exec/ft_pipe.c $(SRC_DIR)exec/ft_here_doc.c $(SRC_DIR)exec/ft_subshell.c $(SRC_DIR)exec/ft_and.c $(SRC_DIR)exec/ft_or.c $(SRC_DIR)exec/ft_cmd.c $(SRC_DIR)exec/ft_in.c $(SRC_DIR)exec/ft_out.c

OBJS = $(patsubst $(SRC_DIR)%.c,$(OBJ_DIR)/%.o,$(SRCS))

# --- bonus --- #
B_NAME = minishell_bonus
BSRC_DIR = ./src/bonus/
B_OBJ_DIR = ./obj_bonus

BSRCS = $(addprefix $(BSRC_DIR), \
		minishell.c init.c exit.c expansor.c \
		syntax/ft_lexer.c syntax/ft_lexer_utils.c syntax/syntax.c syntax/syntax2.c syntax/redirs.c \
		syntax/ft_syntax_redir.c syntax/ft_syntax_logic.c syntax/ft_syntax_init.c syntax/ft_syntax_subshell.c syntax/ft_syntax_pipes.c syntax/ft_syntax_cmd.c \
		builtins/builtins.c builtins/pwd.c builtins/ft_cd.c builtins/ft_exit_builtin.c builtins/ft_unset.c builtins/ft_echo.c builtins/ft_export.c builtins/ft_env.c \
		signals.c wildcards.c \
		utils/ft_shlvl.c utils/get_path.c \
		utils/cleanup/ft_free.c utils/cleanup/ft_free2.c utils/cleanup/free_split.c utils/cleanup/ft_destroyer.c \
		utils/builtins/ft_echo_utils.c utils/builtins/ft_echo_utils2.c utils/builtins/ft_echo_utils3.c \
		utils/export/ft_export_utils.c utils/export/ft_export_utils2.c \
		utils/redirs/ft_add_to_array.c utils/redirs/ft_open.c \
		utils/signals/signals_utils.c \
		utils/env/env.c utils/env/env_utils.c \
		utils/quotes/ft_quotes_utils.c utils/quotes/ft_split_quotes.c \
		utils/expansor/expansor_utils.c utils/expansor/expansor_utils2.c \
		test/print_lexer.c test/print_commands.c test/print_ast.c test/ft_print_args.c \
		exec/ft_exec.c exec/ft_pipe.c exec/ft_here_doc.c exec/ft_subshell.c exec/ft_and.c exec/ft_or.c exec/ft_cmd.c exec/ft_in.c exec/ft_out.c \
)

# This will add _bonus to the object filenames
BOBJS = $(patsubst $(BSRC_DIR)%.c,$(B_OBJ_DIR)/%_bonus.o,$(BSRCS))

CC = cc -g -fsanitize=address,leak

CFLAGS = -I./include -Wall -Werror -Wextra -I./include
RM = rm -rf

# --- libft --- #
LIBFT = ./src/mandatory/utils/libft
EX_LIB = $(LIBFT)/libft.a
B_LIBFT = ./src/bonus/utils/libft
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