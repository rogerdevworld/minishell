# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/16 12:58:52 by rmarrero          #+#    #+#              #
#    Updated: 2025/02/17 11:55:01 by rmarrero         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #
# --- all part --- #
NAME = pipex
SRC_DIR = ./src/
OBJ_DIR = ./obj

SRCS =	$(SRC_DIR)pipex.c $(SRC_DIR)pipex_utils.c $(SRC_DIR)here_doc.c $(SRC_DIR)cmd.c $(SRC_DIR)parse.c

OBJS = $(SRCS:$(SRC_DIR)%.c=$(OBJ_DIR)/%.o)

CC = gcc
CFLAGS = -Wall -Werror -Wextra -I./include
RM = rm -rf

# --- libft --- #
LIBFT = ./libft
EX_LIB = $(LIBFT)/libft.a

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

$(OBJ_DIR)/%.o: $(SRC_DIR)%.c $(HEADER) Makefile | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(EX_LIB) $(OBJECTS) $(HEADER)
	@echo "$(GREEN)Compilando $(NAME)...$(RESET)"
	$(CC) $(CFLAGS) $(OBJECTS) $(EX_LIB) -o $(NAME)
	@echo "$(BLUE)"
	@echo "$(YELLOW)           ($(RESET)__$(YELLOW))\           $(RESET)"
	@echo "$(YELLOW)           ($(RESET)oo$(YELLOW))\\________  $(RESET)"
	@echo "$(RESET)           /|| \\        \\ $(NAME) ready$(RESET)"
	@echo "$(RESET)              ||------w | $(RESET)"
	@echo "$(RESET)              ||       || $(RESET)"
	@echo "$(YELLOW)THE COW MAKES MUUUUUUUUUU!$(RESET)"
	@echo "$(RESET)"

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
