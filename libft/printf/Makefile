CC = cc
CFLAGS = -Wall -Werror -Wextra -I./includes/
RM = rm -f
NAME = libftprintf.a

SRCS = srcs/ft_printf.c srcs/putnbr.c srcs/putstr.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(MAKE) -C ./libft
	cp ./libft/libft.a $(NAME)
	ar rcs $(NAME) $(OBJS)

clean:
	$(MAKE) -C ./libft clean
	$(RM) $(OBJS)

fclean: clean
	$(MAKE) -C ./libft fclean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
