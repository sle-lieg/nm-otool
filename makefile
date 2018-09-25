NM = ft_nm
OTOOL = ft_otool
CC = gcc
FLAGS = -Wall -Wextra -Werror -g

SRC_DIR = src/
OBJ_DIR = obj/
LIB_DIR = libft/
INC_DIR = inc/
DEP_DIR = dep/

LIBFT = $(LIB_DIR)libftprintf.a

SRC_F =	main.c macho_l64.c l64_read_symboles.c l64_print_symboles.c \
			macho_l32.c l32_read_symboles.c l32_print_symboles.c \
			macho_b32.c b32_read_symboles.c b32_print_symboles.c \
			macho_b64.c fat.c
#			macho_b64.c b64_read_symboles.c b64_print_symboles.c \
			fat.c list_64_tools.c

SRC = $(addprefix $(SRC_DIR), $(SRC_F))
OBJ = $(SRC:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)
DEP = $(OBJ:$(OBJ_DIR)%.o=$(DEP_DIR)%.d)

all: createDir $(LIBFT) $(NM)

createDir:
	@test -d $(OBJ_DIR) || mkdir $(OBJ_DIR)
	@test -d $(DEP_DIR) || mkdir $(DEP_DIR)

$(LIBFT):
	make -C $(LIB_DIR)

$(NM): $(OBJ)
	$(CC) $^ -o $@ $(LIBFT)

-include $(DEP)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	$(CC) $(FLAGS) -MMD -MF $(DEP_DIR)$(@F:%.o=%.d) -c $< -o $@ -I $(INC_DIR) -I $(LIB_DIR)$(INC_DIR)

clean:
	@rm -f $(OBJ) $(DEP)
	@make clean -C $(LIB_DIR)

fclean: clean
	@rm -f $(NM) $(OTOOL)
	@make fclean -C $(LIB_DIR)

re: fclean all
