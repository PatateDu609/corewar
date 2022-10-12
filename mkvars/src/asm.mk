BASENAME_ASM	:=	main.c								\
					file.c								\
					op.c								\
					lexer/lexer.c						\
					lexer/tokenizer.c					\
					lexer/dump.c						\
					lexer/token_type/type.c				\
					lexer/token_type/is_string.c		\
					lexer/token_type/is_number.c		\
					lexer/token_type/is_word.c			\
					lexer/token_type/is_header.c		\
					lexer/token_type/is_register.c		\
					lexer/token_type/is_instruction.c	\


SRC_ASM			:=	$(addprefix $(SRC_PATH)/asm/, $(BASENAME_ASM))
OBJ_ASM			:=	$(addprefix $(OBJ_PATH)/asm/, $(BASENAME_ASM:.c=.o))
DEP_ASM			:=	$(addprefix $(OBJ_PATH)/asm/, $(BASENAME_ASM:.c=.d))
