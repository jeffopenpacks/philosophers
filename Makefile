# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: joolim <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/18 17:12:54 by joolim            #+#    #+#              #
#    Updated: 2024/04/04 08:11:22 by joolim           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS = philo.c philo_utils.c time.c hunger_games.c forks.c monitor.c
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
NAME = philo
all: ${NAME}

OBJS = ${SRCS:.c=.o}

$(NAME): ${OBJS}
	${CC} ${CFLAGS} -o $@ $^

%.o: %.c
	${CC} -c ${CFLAGS} $< -o $@

clean:
	rm -f ${OBJS}

fclean: clean
	rm -f ${NAME}

re: fclean all

.PHONY: all clean fclean re
