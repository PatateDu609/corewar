BASENAME_ASM	:=	main.c										\
					file.c										\
					op.c										\
					parser/parse.c								\
					parser/lexer/tokenizer.c					\
					parser/lexer/dump.c							\
					parser/lexer/token_type/type.c				\
					parser/lexer/token_type/is_string.c			\
					parser/lexer/token_type/is_number.c			\
					parser/lexer/token_type/is_word.c			\
					parser/lexer/token_type/is_header.c			\
					parser/lexer/token_type/is_register.c		\
					parser/lexer/token_type/is_instruction.c	\


SRC_ASM			:=	$(addprefix $(SRC_PATH)/asm/, $(BASENAME_ASM))
OBJ_ASM			:=	$(addprefix $(OBJ_PATH)/asm/, $(BASENAME_ASM:.c=.o))
DEP_ASM			:=	$(addprefix $(OBJ_PATH)/asm/, $(BASENAME_ASM:.c=.d))
