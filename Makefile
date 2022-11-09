include mkvars/vars.mk
include mkvars/decorators.mk
include mkvars/src/common.mk
include mkvars/src/asm.mk
include mkvars/src/corewar.mk

all:				$(NAME_ASM) $(NAME_COREWAR)

$(OBJ_PATH)/%.o:	$(SRC_PATH)/%.c
				@mkdir -p $(dir $@)
				@$(ECHO) "$(INFO) Compiling" $@ "$(RESET_COLOR)"
				@$(CC) -c $(CFLAGS) -MMD -o $@ $<


-include $(DEP) $(DEP_ASM) $(DEP_COREWAR)


$(NAME_ASM):	$(OBJ) $(OBJ_ASM) $(LIBFT)
				@$(ECHO) "$(DONE) Linking $@ $(RESET_COLOR)"
				@$(CC) -o $(NAME_ASM) $(OBJ) $(OBJ_ASM) $(LDFLAGS)

$(NAME_COREWAR):	$(OBJ) $(OBJ_COREWAR) $(LIBFT)
				@$(ECHO) "$(DONE) Linking $@ $(RESET_COLOR)"
				@$(CC) -o $(NAME_COREWAR) $(OBJ) $(OBJ_COREWAR) $(LDFLAGS)

clean:
				@$(ECHO) "$(ERROR) Cleaning object files$(RESET_COLOR)"
				@rm -f $(OBJ) $(OBJ_ASM) $(OBJ_COREWAR)
				@$(MAKE) -C $(LIBFT_PATH) clean

fclean:			clean
				@$(ECHO) "$(ERROR) Cleaning executables and libraries$(RESET_COLOR)"
				@rm -rf $(OBJ_PATH)
				@rm -f $(NAME_ASM) $(NAME_COREWAR)
				@$(MAKE) -C $(LIBFT_PATH) fclean

re:					fclean all

$(LIBFT):
				@$(ECHO) "$(INFO) Compiling the libft $(RESET_COLOR)"
				@$(MAKE) -C $(LIBFT_PATH)

.PHONY:		all re fclean clean
