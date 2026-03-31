# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wtang <wtang@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/08 14:12:52 by wtang             #+#    #+#              #
#    Updated: 2026/01/16 16:09:14 by wtang            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

SRCS = philo.c init.c routine.c routine_utils.c monitor.c utils.c
OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)

RM = rm -f

all: $(NAME)

$(NAME): $(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -o $@

%.o: %.c
		$(CC) -MMD -MP $(CFLAGS) -c $< -o $@

clean:
		$(RM) $(OBJS) $(DEPS)

fclean: clean
		$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re

-include $(DEPS)