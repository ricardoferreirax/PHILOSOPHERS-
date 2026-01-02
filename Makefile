NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror -g
RM = rm -rf

INCLUDE = includes

SRCS = \
	main.c \
	srcs/philo_routine.c \
	srcs/check_death.c \
	srcs/init_philos.c \
	srcs/philo_forks.c \
	srcs/parsing.c \
	srcs/utils/cleanup.c \
	srcs/utils/philo_utils.c \
	srcs/utils/libft_one.c \
	srcs/utils/libft_two.c \

OBJS = $(SRCS:.c=.o)

RED = \033[1;37;41m
GREEN = \033[1;30;42m
OFF = \033[0m

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@
	@echo -e "$(GREEN) ./philo created! $(OFF)"

%.o: %.c $(INCLUDE)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)
	@echo -e "$(RED) Object Files Cleaned! $(OFF)"

fclean: clean
	$(RM) $(NAME)
	@echo -e "$(GREEN) All Cleaned! $(OFF)" 

re: fclean $(NAME)

.PHONY: all clean fclean re