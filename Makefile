NAME = philo

SRCS = philo.c utils.c \

OBJS = $(SRCS:.c=.o)

CC = clang -Wall -Wextra -Werror -pthread -fsanitize=address -g

RM = rm -rf

.o.c = $(CC) -c $< -o ${$<.c=.o}

all: ${NAME}

$(NAME): ${OBJS}

clean:
	@${RM} ${OBJS}
	@${RM} ${NAME}
	@echo "$(_RED) Everything was cleaned. $(_END)"

fclean: clean
	@${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re