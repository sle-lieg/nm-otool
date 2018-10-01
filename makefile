NM = ft_nm
OTOOL = ft_otool
CC = gcc
FLAGS = -Wall -Wextra -Werror -g

################# DIRECTORIES #################
NM_SRC_DIR = nm_src/
NM_OBJ_DIR = nm_obj/

OTOOL_SRC_DIR = otool_src/
OTOOL_OBJ_DIR = otool_obj/

COMMON_SRC_DIR = common_src/

LIB_DIR = libft/
INC_DIR = inc/
DEP_DIR = dep/

################# LIBFT #################
LIBFT = $(LIB_DIR)libftprintf.a

################# NM FILES ##############
NM_SRC_F =	macho_32.c read_symboles_32.c print_symboles_32.c \
				macho_64.c read_symboles_64.c print_symboles_64.c

################# OTOOL FILES ###########
OTOOL_SRC_F = macho_32.c macho_64.c

################# COMMON FILES ##########
COMMON_SRC_F = main.c fat.c fat_64.c archive.c tools.c

COMMON_SRC := $(addprefix $(COMMON_SRC_DIR), $(COMMON_SRC_F))

NM_SRC = $(addprefix $(NM_SRC_DIR), $(NM_SRC_F))
NM_OBJ = $(NM_SRC:$(NM_SRC_DIR)%.c=$(NM_OBJ_DIR)%.o)
NM_OBJ := $(NM_OBJ) $(COMMON_SRC:$(COMMON_SRC_DIR)%.c=$(NM_OBJ_DIR)%.o)

OTOOL_SRC = $(addprefix $(OTOOL_SRC_DIR), $(OTOOL_SRC_F))
OTOOL_OBJ = $(OTOOL_SRC:$(OTOOL_SRC_DIR)%.c=$(OTOOL_OBJ_DIR)%.o)
OTOOL_OBJ := $(OTOOL_OBJ) $(COMMON_SRC:$(COMMON_SRC_DIR)%.c=$(OTOOL_OBJ_DIR)%.o)

DEP = $(NM_OBJ:$(NM_OBJ_DIR)%.o=$(DEP_DIR)%.d)

all: createDir $(LIBFT) $(NM) $(OTOOL)

createDir:
	@test -d $(NM_OBJ_DIR) || mkdir $(NM_OBJ_DIR)
	@test -d $(OTOOL_OBJ_DIR) || mkdir $(OTOOL_OBJ_DIR)
	@test -d $(DEP_DIR) || mkdir $(DEP_DIR)

$(LIBFT):
	make -C $(LIB_DIR)

-include $(DEP)

################# COMPILE NM #################
$(NM): $(NM_OBJ)
	$(CC) $^ -o $@ $(LIBFT)

$(NM_OBJ_DIR)%.o: $(NM_SRC_DIR)%.c
	$(CC) $(FLAGS) -MMD -MF $(DEP_DIR)$(@F:%.o=%.d) -c $< -o $@ -I $(INC_DIR) -I $(LIB_DIR)$(INC_DIR)

$(NM_OBJ_DIR)%.o: $(COMMON_SRC_DIR)%.c
	$(CC) $(FLAGS) -MMD -MF $(DEP_DIR)$(@F:%.o=%.d) -c $< -o $@ -I $(INC_DIR) -I $(LIB_DIR)$(INC_DIR)


################ COMPILE OTOOL ################
$(OTOOL): $(OTOOL_OBJ)
	$(CC) $^ -o $@ $(LIBFT)

$(OTOOL_OBJ_DIR)%.o: $(OTOOL_SRC_DIR)%.c
	$(CC) $(FLAGS) -MMD -MF $(DEP_DIR)$(@F:%.o=%.d) -c $< -o $@ -I $(INC_DIR) -I $(LIB_DIR)$(INC_DIR)

$(OTOOL_OBJ_DIR)%.o: $(COMMON_SRC_DIR)%.c
	$(CC) $(FLAGS) -MMD -MF $(DEP_DIR)$(@F:%.o=%.d) -c $< -o $@ -I $(INC_DIR) -I $(LIB_DIR)$(INC_DIR)

clean:
	@rm -f $(NM_OBJ) $(OTOOL_OBJ) $(DEP)
	@make clean -C $(LIB_DIR)

fclean: clean
	@rm -f $(NM) $(OTOOL)
	@make fclean -C $(LIB_DIR)

re: fclean all
