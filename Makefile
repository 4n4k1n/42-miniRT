MAKEFLAGS += -s

NAME	:= miniRT
CFLAGS	:= -Wall -Wextra -Ofast -flto -march=native -mtune=native \
  -funroll-loops -fprefetch-loop-arrays -fpeel-loops -funswitch-loops -ftracer \
  -ftree-vectorize -ftree-slp-vectorize -ftree-loop-distribution -fivopts \
  -fgraphite-identity -floop-nest-optimize -floop-interchange -floop-strip-mine -floop-block -floop-unroll-and-jam \
  -fstrict-aliasing -fweb -frename-registers -fira-loop-pressure -fira-region=all -fira-hoist-pressure \
  -fsched-pressure -fsched-spec -fsched-spec-load \
  -falign-functions=32 -falign-loops=32 -falign-jumps=32 \
  -freorder-blocks -freorder-blocks-and-partition -freorder-functions \
  -fdata-sections -ffunction-sections -Wl,--gc-sections \
  -fno-plt -fno-semantic-interposition \
  -fomit-frame-pointer -fmerge-all-constants \
  -ffast-math -fno-math-errno -fno-trapping-math -freciprocal-math \
  -fassociative-math -ffinite-math-only -fno-signed-zeros \
  -fipa-pta -fipa-cp-clone -fipa-sra -fipa-pure-const -fipa-reference \
  -fdevirtualize -fdevirtualize-speculatively \
  -fno-stack-protector -fno-exceptions -fwhole-program -fsanitize=address,undefined -g
OBJ_DIR = objs
CC = cc

SRC = src/main.c \
		src/math/vec_add.c \
		src/math/vec_cpy.c \
		src/math/vec_cross.c \
		src/math/vec_divide.c \
		src/math/vec_dot.c \
		src/math/vec_init.c \
		src/math/vec_multiply.c \
		src/math/vec_overload.c \
		src/math/vec_sqrt.c \
		src/math/vec_squared.c \
		src/math/vec_sub.c \
		src/math/random.c \
		src/math/vec_zero.c \
		src/math/vec_reflect.c \
		src/utils/color.c \
		src/parsing/error.c \
		src/parsing/list.c \
		src/parsing/parsing.c \
		src/parsing/utils.c \
		src/parsing/validation_obj.c \
		src/parsing/validation.c \
		src/parsing/validators.c \
		src/logic/hit_sphere.c \
		src/logic/hittable.c \
		src/logic/monte_carlo_aa.c \
		src/logic/camera.c \
		src/logic/metal.c \
		src/logic/lambertian.c \
		src/math/random_vec.c \
		src/utils/mlx_hooks.c

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

$(OBJ_DIR)/%.o: src/%.c inc/mini_rt.h
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
