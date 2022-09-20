include mkvars/vars.mk
include mkvars/decorators.mk
include mkvars/src/common.mk
include mkvars/src/asm.mk
include mkvars/src/corewar.mk

# TODO: Add specific compiler flag for each (include path for example)

all:				$(NAME_ASM) $(NAME_COREWAR)

$(OBJ_PATH)/%.o:	$(SRC_PATH)/%.c
				@mkdir -p $(dir $@)
				@/bin/echo -e "$(INFO) Compiling" $@ "$(RESET_COLOR)"
				@$(CC) -c $(CFLAGS) -o $@ $<

-include $(DEP)


$(NAME_ASM):	$(OBJ) $(OBJ_ASM) $(LIBFT)
				@/bin/echo -e "$(DONE) Linking $@ $(RESET_COLOR)"
				@$(CC) -o $(NAME_ASM) $(OBJ) $(OBJ_ASM) $(LDFLAGS)

$(NAME_COREWAR):	$(OBJ) $(OBJ_COREWAR) $(LIBFT)
				@/bin/echo -e "$(DONE) Linking $@ $(RESET_COLOR)"
				@$(CC) -o $(NAME_COREWAR) $(OBJ) $(OBJ_COREWAR) $(LDFLAGS)

clean:
				@/bin/echo -e "$(ERROR) Cleaning object files$(RESET_COLOR)"
				@rm -f $(OBJ) $(OBJ_ASM) $(OBJ_COREWAR)
				@make -C $(LIBFT_PATH) clean

fclean:			clean
				@/bin/echo -e "$(ERROR) Cleaning executables and libraries$(RESET_COLOR)"
				@rm -rf $(OBJ_PATH)
				@rm -f $(NAME_ASM) $(NAME_COREWAR)
				@make -C $(LIBFT_PATH) fclean

re:					fclean all

$(LIBFT):
				@/bin/echo -e "$(INFO) Compiling the libft $(RESET_COLOR)"
				@make -C $(LIBFT_PATH)

.PHONY:		all re fclean clean
