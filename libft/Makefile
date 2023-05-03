# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/09 17:43:15 by hyobicho          #+#    #+#              #
#    Updated: 2023/04/29 22:04:29 by yunjcho          ###   ########seoul.kr   #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRCS = \
ft_strchr_idx.c \
ft_strcpy.c \
ft_strlowcase.c \
ft_error.c \
ft_freestrs.c \
ft_strjoin_three.c \
ft_isalpha.c \
ft_isdigit.c \
ft_isalnum.c \
ft_isascii.c \
ft_isprint.c \
ft_strlen.c \
ft_memset.c \
ft_bzero.c \
ft_memcpy.c \
ft_memmove.c \
ft_strlcpy.c \
ft_strlcat.c \
ft_toupper.c \
ft_tolower.c \
ft_strchr.c \
ft_strrchr.c \
ft_strncmp.c \
ft_strcmp.c \
ft_memchr.c \
ft_memcmp.c \
ft_strnstr.c \
ft_atoi.c \
ft_calloc.c \
ft_strdup.c \
ft_strjoin.c \
ft_substr.c \
ft_split.c \
ft_strtrim.c \
ft_itoa.c \
ft_strmapi.c \
ft_striteri.c \
ft_putchar_fd.c \
ft_putstr_fd.c \
ft_putnbr_fd.c \
ft_putendl_fd.c
M_OBJ = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(M_OBJ)
	ar rc $@ $^

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(M_OBJ) $(B_OBJ)

fclean: clean
	rm -f $(NAME)

re: 
	make fclean
	make all

.PHONY: all re clean fclean