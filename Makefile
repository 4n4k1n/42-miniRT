MAKEFLAGS += -s

NAME	:= miniRT
CFLAGS	:= -Wall -Wextra -Werror -Ofast -flto -march=native -mtune=native \
  -funroll-loops -fprefetch-loop-arrays -fpeel-loops -funswitch-loops -ftracer \
  -ftree-vectorize -ftree-slp-vectorize -ftree-loop-distribution -fivopts \
  -fgraphite-identity -floop-nest-optimize -floop-interchange -floop-strip-mine -floop-block -floop-unroll-and-jam \
  -fstrict-aliasing -fweb -frename-registers -fira-loop-pressure -fira-region=all -fira-hoist-pressure \
  -fsched-pressure -fsched-spec -fsched-spec-load \
  -falign-functions=32 -falign-loops=32 -falign-jumps=32 -falign-labels=32 \
  -freorder-blocks -freorder-blocks-and-partition -freorder-functions \
  -fdata-sections -ffunction-sections -Wl,--gc-sections \
  -fno-plt -fno-semantic-interposition \
  -fomit-frame-pointer -fmerge-all-constants \
  -ffast-math -fno-math-errno -fno-trapping-math -freciprocal-math \
  -fassociative-math -ffinite-math-only -fno-signed-zeros \
  -fipa-pta -fipa-cp-clone -fipa-sra -fipa-pure-const -fipa-reference \
  -fdevirtualize -fdevirtualize-speculatively \
  -fno-stack-protector -fno-exceptions -fwhole-program -pthread \
  -fmodulo-sched -fmodulo-sched-allow-regmoves \
  -fsplit-loops -fpredictive-commoning -ftree-loop-im -ftree-loop-if-convert \
  -fgcse-sm -fgcse-las -fvariable-expansion-in-unroller -funroll-all-loops \
  -Wno-unused-result

LDFLAGS := -Wl,-O2 -Wl,--sort-common -Wl,--as-needed -no-pie
OBJ_DIR = objs
CC = gcc

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
		src/math/vec_refract.c \
		src/math/vec_normalize.c \
		src/math/surface_bias.c \
		src/math/rgb_add.c \
		src/math/rgb_modulate.c \
		src/math/rgb_multiply.c \
		src/utils/color.c \
		src/parsing/error.c \
		src/parsing/list.c \
		src/parsing/parsing.c \
		src/parsing/utils.c \
		src/parsing/validation_mat.c \
		src/parsing/mat_token.c \
		src/parsing/mat_token_helpers.c \
		src/parsing/validation.c \
		src/parsing/validators.c \
		src/parsing/bump.c \
		src/parsing/parsing_sphere.c \
		src/parsing/parsing_plane.c \
		src/parsing/parsing_cylinder.c \
		src/parsing/parsing_pyramid.c \
		src/parsing/parsing_cone.c \
		src/parsing/parsing_triangle.c \
		src/parsing/debug_print.c \
		src/logic/hit_sphere.c \
		src/logic/hit_plane.c \
		src/logic/hit_tower.c \
		src/logic/hit_tower_utils.c \
		src/logic/hit_pyramid.c \
		src/logic/hit_pyramid_utils.c \
		src/logic/hit_pyramid_basis.c \
		src/logic/hit_cone.c \
		src/logic/hit_cone_utils.c \
		src/logic/hit_cone_calc.c \
		src/logic/hit_triangle.c \
		src/logic/hittable.c \
		src/logic/aabb.c \
		src/logic/aabb_utils.c \
		src/logic/bounds.c \
		src/logic/bounds_cones.c \
		src/logic/bounds_simple.c \
		src/logic/bounds_dispatch.c \
		src/logic/bvh_build.c \
		src/logic/bvh_hit.c \
		src/logic/bvh_cleanup.c \
		src/logic/bvh_utils.c \
		src/logic/ray.c \
		src/logic/ray_helpers.c \
		src/logic/ray_lighting.c \
		src/logic/ray_shadow.c \
		src/logic/render.c \
		src/logic/monte_carlo_aa.c \
		src/logic/monte_carlo_aa_helpers.c \
		src/logic/bump.c \
		src/logic/camera.c \
		src/logic/metal.c \
		src/logic/lambertian.c \
		src/logic/phong.c \
		src/logic/dielectric.c \
		src/logic/dielectric_utils.c \
		src/math/random_vec.c \
		src/utils/mlx_hooks.c \
		src/parsing/debug.c \
		src/utils/threads.c \
		src/utils/threads_init.c \
		src/utils/threads_utils.c \
		src/utils/error.c \
		src/server/master.c \
		src/server/setup_master.c \
		src/server/worker.c \
		src/server/worker_accept.c \
		src/server/worker_run.c \
		src/server/worker_tile.c \
		src/server/worker_utils.c \
		src/server/socket.c \
		src/server/queue.c \
		src/server/ip.c \
		src/server/broadcast.c \
		src/server/send/send_all.c \
		src/server/send/send_file.c \
		src/server/send/send_header.c \
		src/server/send/send_tile_assignment.c \
		src/server/send/send_tile_result.c \
		src/server/send/send_settings.c \
		src/server/send/send_update.c \
		src/server/recive/recive_all.c \
		src/server/recive/recive_header.c \
		src/server/recive/recive_scene_file.c \
		src/server/recive/recive_tile_assignment.c \
		src/server/recive/recive_tile_result.c \
		src/server/recive/recive_settings.c \
		src/server/recive/recive_update.c

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
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) $(LIBMLX42) $(LIBFT) -o $(NAME)

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

re:
	$(MAKE) fclean
	$(MAKE) all

.PHONY: all clean fclean re
