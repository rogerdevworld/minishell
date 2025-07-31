# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/16 12:58:52 by rmarrero          #+    #+#              #
#    Updated: 2025/07/29 18:13:37 by arhea            ###   ########.fr       #
#                                                                              #
# **************************************************************************** #

# --- Mandatory --- #
NAME = minishell
SRC_DIR = ./src/mandatory/
OBJ_DIR = ./obj
HEADER = ./include/minishell.h

SRCS = $(SRC_DIR)minishell.c $(SRC_DIR)main_loop.c $(SRC_DIR)loop_processing.c $(SRC_DIR)loop_execution.c $(SRC_DIR)expansor.c \
        $(SRC_DIR)syntax/ft_lexer.c $(SRC_DIR)syntax/ft_lexer_2.c $(SRC_DIR)syntax/ft_lexer_utils.c $(SRC_DIR)syntax/syntax.c $(SRC_DIR)syntax/syntax2.c $(SRC_DIR)syntax/syntax3.c $(SRC_DIR)syntax/redirs.c \
        $(SRC_DIR)syntax/ft_syntax_redir.c $(SRC_DIR)syntax/ft_syntax_logic.c $(SRC_DIR)syntax/ft_syntax_init.c $(SRC_DIR)syntax/ft_syntax_subshell.c $(SRC_DIR)syntax/ft_syntax_pipes.c $(SRC_DIR)syntax/ft_syntax_cmd.c \
        $(SRC_DIR)builtins/ft_builtins.c $(SRC_DIR)builtins/ft_pwd.c $(SRC_DIR)builtins/ft_cd.c $(SRC_DIR)builtins/ft_exit_builtin.c $(SRC_DIR)builtins/ft_unset.c $(SRC_DIR)builtins/ft_echo.c $(SRC_DIR)builtins/ft_export.c $(SRC_DIR)builtins/ft_env.c \
        $(SRC_DIR)signals.c \
        $(SRC_DIR)wildcards.c \
        $(SRC_DIR)utils/wildcards/wildcards_utils.c \
        $(SRC_DIR)utils/wildcards/wildcards_expand.c \
        $(SRC_DIR)utils/wildcards/wildcards_redir.c \
        $(SRC_DIR)utils/ft_shlvl.c $(SRC_DIR)utils/get_path.c \
        $(SRC_DIR)utils/cleanup/ft_free.c $(SRC_DIR)utils/cleanup/ft_free2.c $(SRC_DIR)utils/cleanup/ft_free3.c $(SRC_DIR)utils/cleanup/free_split.c \
        $(SRC_DIR)utils/builtins/ft_echo_utils.c $(SRC_DIR)utils/builtins/ft_echo_utils2.c $(SRC_DIR)utils/builtins/ft_echo_utils3.c \
        $(SRC_DIR)utils/export/ft_export_utils.c $(SRC_DIR)utils/export/ft_export_utils2.c \
        $(SRC_DIR)utils/redirs/ft_add_to_array.c $(SRC_DIR)utils/redirs/ft_open.c $(SRC_DIR)utils/redirs/utils.c \
        $(SRC_DIR)utils/signals/signals_utils.c \
        $(SRC_DIR)utils/env/env.c $(SRC_DIR)utils/env/env_utils.c \
        $(SRC_DIR)utils/quotes/ft_quotes_utils.c $(SRC_DIR)utils/quotes/ft_split_quotes.c\
        $(SRC_DIR)utils/expansor/expansor_utils.c $(SRC_DIR)utils/expansor/expansor_utils2.c \
        $(SRC_DIR)test/print_lexer.c $(SRC_DIR)test/print_commands.c $(SRC_DIR)test/print_ast.c $(SRC_DIR)test/ft_print_args.c \
        $(SRC_DIR)exec/ft_exec.c $(SRC_DIR)exec/ft_pipe.c $(SRC_DIR)exec/ft_here_doc.c \
        $(SRC_DIR)exec/ft_here_doc_utils.c $(SRC_DIR)exec/ft_here_doc_expand.c \
        $(SRC_DIR)exec/ft_subshell.c $(SRC_DIR)exec/ft_and.c $(SRC_DIR)exec/ft_or.c $(SRC_DIR)exec/ft_cmd.c $(SRC_DIR)exec/ft_in.c $(SRC_DIR)exec/ft_out.c

OBJS = $(patsubst $(SRC_DIR)%.c,$(OBJ_DIR)/%.o,$(SRCS))

CC = cc -g -fsanitize=address,leak
CFLAGS = -I./include -Wall -Werror -Wextra -I./include
RM = rm -rf

# --- libft --- #
LIBFT_REPO_URL = https://github.com/rogerdevworld/libftall.git
LIBFT_DIR = ./src/libft
EX_LIB = $(LIBFT_DIR)/libft.a

# --- Colores --- #
RED     = \033[31m
GREEN   = \033[32m
YELLOW  = \033[33m
BLUE    = \033[34m
RESET   = \033[0m

# --- Targets Principales --- #
all: libft $(NAME)

$(NAME): $(OBJS) $(EX_LIB)
	@echo "$(GREEN)Compilando $(NAME)...$(RESET)"
	$(CC) $(CFLAGS) $(OBJS) $(EX_LIB) -o $(NAME) -lreadline -g
	@echo ""
	@echo ""
	@echo "# **************************************************************************** #"
	@echo "#                                                                              #"
	@echo "#                                                         :::      ::::::::    #"
	@echo "#    Minishell                                          :+:      :+:    :+:    #"
	@echo "#                                                     +:+ +:+         +:+      #"
	@echo "#    By: rmarrero rmarrero@student.42barcelona.com  +#+  +:+       +#+         #"
	@echo "#                                                 +#+#+#+#+#+   +#+            #"
	@echo "#    Created: 2024/09/16 12:58:52 by rmarrero          #+    #+#              #"
	@echo "#    Updated: 2025/07/29 13:04:23 by rmarrero         ###   ########.fr       #"
	@echo "#                                                                              #"
	@echo "# **************************************************************************** #"
	@echo ""
	@echo ""
	@echo ""
	@echo ""
	@echo "                          %%%%%%%%%%                                            "
	@echo "                        +%%%%%%%%%%%%%%%%%%%%+                                  "
	@echo "                      %%%%%%%%%%%%%%%%%%%%%%%%%%%%                              "
	@echo "                    %%%%%%%%%......%%......%%%%%%%%%                            "
	@echo "                  %%%%%%:..%%......%%......%%..:%%%%%%                          "
	@echo "                %%%%%%......%......%%......%......%%%%%%                        "
	@echo "                %%%%%%%......%%.....%%.....%%......%%%%%%%                      "
	@echo "               %%%%..%%......%.....%%.....%......%%..%%%%%                      "
	@echo "              %%%%....%%.....%*....##....*%.....%%....%%%%%                     "
	@echo "              =%%%%......%*.....%....##....%.....*%......%%%%=                    "
	@echo "              %%%%%%......%.....%....--....%.....%......%%%%%%                     "
	@echo "              %%%.%%:.....%.....%........%.....%......%%.%%%%                     "
	@echo "              %%%%%...%%.....%....%........%....%.....%%...%%%%%                   "
	@echo "              %%%%#.....%:....%................%.....%.....#%%%%                   "
	@echo "              %%%%%.......%....%...%......%...%....%.......%%%%%                   "
	@echo "               %%%%%=......=....%............%....=......=%%%%%                   "
	@echo "                 %%%%%%%......%...=..:....:..=...%......%%%%%%%                    "
	@echo "                   #%%%%%%#.....-..............-.....#%%%%%%#                     "
	@echo "                     %%%%%%........................%%%%%%                          "
	@echo "                       %%%%........................%%%%.                           "
	@echo "                       %%%%........................%%%%                            "
	@echo "                       %%%%........................%%%%                            "
	@echo "                       %%%%%%%%%%%%%%%++%%%%%%%%%%%%%%%                            "
	@echo "                       %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%#                             "
	@echo "                                 %%%%                                              "
	@echo "                                                                                 "
	@echo "                          You are beyond the $(NAME)                          "
	@echo "$(RESET)"

# --- Targets Auxiliares --- #
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/builtins
	@mkdir -p $(OBJ_DIR)/exec
	@mkdir -p $(OBJ_DIR)/syntax
	@mkdir -p $(OBJ_DIR)/utils
	@mkdir -p $(OBJ_DIR)/utils/cleanup
	@mkdir -p $(OBJ_DIR)/utils/builtins
	@mkdir -p $(OBJ_DIR)/utils/export
	@mkdir -p $(OBJ_DIR)/utils/redirs
	@mkdir -p $(OBJ_DIR)/utils/env
	@mkdir -p $(OBJ_DIR)/utils/signals
	@mkdir -p $(OBJ_DIR)/utils/quotes
	@mkdir -p $(OBJ_DIR)/utils/expansor
	@mkdir -p $(OBJ_DIR)/utils/wildcards
	@mkdir -p $(OBJ_DIR)/test

$(OBJ_DIR)/%.o: $(SRC_DIR)%.c $(HEADER) Makefile | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

libft:
	@echo "$(GREEN)Verificando y compilando libft...$(RESET)"
	@if [ ! -d "$(LIBFT_DIR)" ]; then \
		echo "$(YELLOW)Clonando libft de $(LIBFT_REPO_URL)...$(RESET)"; \
		git clone $(LIBFT_REPO_URL) $(LIBFT_DIR); \
	fi
	@make -C $(LIBFT_DIR)

# --- Reglas de Limpieza --- #
clean:
	@echo "$(GREEN)Eliminando archivos objeto...$(RESET)"
	$(RM) $(OBJ_DIR)
	@make clean -C $(LIBFT_DIR)

fclean: clean
	@echo "$(GREEN)Eliminando ejecutable y librerías...$(RESET)"
	$(RM) -f $(NAME)
	$(RM) -rf $(LIBFT_DIR)

re: fclean all

.PHONY: all clean fclean re libft