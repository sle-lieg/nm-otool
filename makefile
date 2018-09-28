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

SRC_F =	main.c \
			macho_32.c read_symboles_32.c print_symboles_32.c \
			macho_64.c read_symboles_64.c print_symboles_64.c \
			fat.c fat_64.c archive.c

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
