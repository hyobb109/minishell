# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/18 19:16:02 by yunjcho           #+#    #+#              #
#    Updated: 2023/04/16 22:27:35 by hyobicho         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc #-g
CFLAGS = -Wall -Wextra -Werror
R_FLAG =  -lreadline -L${HOME}/.brew/opt/readline/lib
R_INC = -I${HOME}/.brew/opt/readline/include
LIBFT_FLAG = -Llibft -lft
LIBFT_DIR = ./libft/
SRCS = \
ft_main.c \
pipex/deque_utils.c \
pipex/deque.c \
pipex/process_children.c \
pipex/process_parents.c \
pipex/builtins.c \
pipex/find_paths.c
M_OBJ = $(SRCS:.c=.o)

ifdef SANITIZE
	CFLAGS += -fsanitize=address
endif

all: $(NAME)

$(NAME): $(M_OBJ)
	@make -C $(LIBFT_DIR)
	@$(CC) $(M_OBJ) $(LIBFT_FLAG) $(R_FLAG) $(CFLAGS) -o $(NAME)

%.o : %.c
	@$(CC) -c $(CFLAGS) $(R_INC) $< -o $@

clean:
	@make -C $(LIBFT_DIR) clean
	@rm -f $(M_OBJ)

fclean: clean
	@make -C $(LIBFT_DIR) fclean
	@rm -f $(NAME)

re: 
	make fclean
	make all

sanitize:
	make SANITIZE=1 re

.PHONY: all re clean fclean sanitize