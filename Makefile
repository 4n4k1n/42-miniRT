NAME	:= miniRT
CFLAGS	:= -Wall -Wextra -Werror -Wunreachable-code -Ofast -march=native -mtune=native -flto -funroll-loops
LIBMLX	:= ./MLX42

HEADERS	:= -I ./inc -I $(LIBMLX)/include
LIBS	:= $(LIBMLX)/build/libmlx42.a -ldl -lglfw -pthread -lm

SRC_DIR = src
OBJ_DIR = obj

SRCS	:= main.c

OBJS := $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

all: libmlx $(NAME)

libmlx:
	@cmake $(LIBMLX) -B $(LIBMLX)/build && make -C $(LIBMLX)/build -j4

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c inc/miniRT.h
	$(CC) $(CFLAGS) $(HEADERS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(NAME): $(OBJ_DIR) $(OBJS)
	$(CC) $(OBJS) $(LIBS) $(HEADERS) -o $(NAME)

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(LIBMLX)/build

fclean: clean
	rm -rf $(NAME)

re: clean all

.PHONY: all clean fclean re libmlx