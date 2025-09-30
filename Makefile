NAME	:= miniRT
CFLAGS	:= -Wall -Wextra -Werror -Wunreachable-code -Ofast -march=native -mtune=native -flto -funroll-loops -lm
LIBMLX	:= ./MLX42

HEADERS	:= -I ./inc -I $(LIBMLX)/include
LIBS	:= $(LIBMLX)/build/libmlx42.a -ldl -lglfw -pthread -lm

SRC_DIR = src
OBJ_DIR = obj

SRCS	:= main.c \
		math/vec_add.c \
		math/vec_cpy.c \
		math/vec_cross.c \
		math/vec_divide.c \
		math/vec_dot.c \
		math/vec_init.c \
		math/vec_multiply.c \
		math/vec_overload.c \
		math/vec_sqrt.c \
		math/vec_squared.c \
		math/vec_sub.c \
		utils/image.c \
		utils/ray.c \
		parsing/error.c \
		parsing/list.c \
		parsing/parsing.c \
		parsing/utils.c \
		parsing/validation_obj.c \
		parsing/validation.c \
		parsing/validators.c

OBJS := $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

all: libmlx $(NAME)

libmlx:
	@cmake $(LIBMLX) -B $(LIBMLX)/build && make -C $(LIBMLX)/build -j4

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(HEADERS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR) $(OBJ_DIR)/math $(OBJ_DIR)/utils

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(LIBS) $(HEADERS) -o $(NAME)

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(LIBMLX)/build

fclean: clean
	rm -rf $(NAME)

re: clean all

.PHONY: all clean fclean re libmlx