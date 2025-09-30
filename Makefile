MAKEFLAGS += -s

NAME	:= miniRT
CFLAGS	:= -Wall -Wextra -Werror -flto
OBJ_DIR = objs
CC = cc

SRC = src/main.c src/parsing/error.c src/parsing/utils.c src/parsing/parsing.c \
	src/parsing/validation.c src/parsing/validators.c src/parsing/list.c \
	src/parsing/validation_obj.c src/parsing/debug.c

OBJ = $(SRC:src/%.c=$(OBJ_DIR)/%.o)

UNAME_S := $(shell uname -s)

LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

MLX42_DIR = ./MLX42

ifeq ($(UNAME_S),Linux)
	LIBMLX42 = $(MLX42_DIR)/build/libmlx42.a -ldl -lglfw -pthread -lm
	CFLAGS += -D LINUX
else ifeq ($(UNAME_S),Darwin)
	LIBMLX42 = $(MLX42_DIR)/build/libmlx42.a -lglfw -framework Cocoa -framework OpenGL -framework IOKit
	CFLAGS += -D OSX
else
	$(error OS not supported: $(UNAME_S))
endif

CFLAGS += -I./inc -I$(MLX42_DIR)/include

all: $(LIBFT) $(LIBMLX42) $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBMLX42) $(LIBFT) -o $(NAME)

$(OBJ_DIR)/%.o: src/%.c inc/miniRT.h
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(LIBMLX42):
	@if [ ! -d $(MLX42_DIR) ]; then \
		git clone https://github.com/codam-coding-college/MLX42.git \
		$(MLX42_DIR); \
	fi
	@if [ ! -f $(MLX42_DIR)/build/libmlx42.a ]; then \
		cmake $(MLX42_DIR) -B $(MLX42_DIR)/build && \
		cmake --build $(MLX42_DIR)/build -j4; \
	fi

clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) clean -C $(LIBFT_DIR)

fclean: clean
	rm -f $(NAME)
	$(MAKE) fclean -C $(LIBFT_DIR)
	@rm -rf $(MLX42_DIR)

re:
	$(MAKE) fclean
	$(MAKE) all

.PHONY: all clean fclean re