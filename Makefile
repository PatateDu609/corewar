include mkvars/vars.mk
include mkvars/decorators.mk
include mkvars/src/common.mk
include mkvars/src/asm.mk
include mkvars/src/corewar.mk

# TODO: Add specific compiler flag for each (include path for example)

all:				$(NAME_ASM) $(NAME_COREWAR)

$(OBJ_PATH)/%.o:	$(SRC_PATH)/%.c
					@mkdir -p $(dir $@)
					@/bin/echo -e "$(INFO) Compiling" $@ $(RESET_COLOR)
					@$(CC) -c $(CFLAGS) -o $@ $<

-include $(DEP)


$(NAME_ASM):		$(OBJ) $(OBJ_ASM)
					@$(CC) -o $(NAME_ASM) $(OBJ) $(OBJ_ASM)

$(NAME_COREWAR):	$(OBJ) $(OBJ_COREWAR)
					@$(CC) -o $(NAME_COREWAR) $(OBJ) $(OBJ_COREWAR)

clean:
					@rm -f $(OBJ) $(OBJ_ASM) $(OBJ_COREWAR)

fclean:				clean
					@rm -rf $(OBJ_PATH)
					@rm -f $(NAME_ASM) $(NAME_COREWAR)

re:					fclean all

.PHONY:		all re fclean clean
