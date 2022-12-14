BASENAME_ASM	:=	main.c										\
					file.c										\
					op.c										\
					encoder/encode.c							\
					encoder/get_size.c							\
					encoder/params.c							\
					encoder/file.c								\
					encoder/labels.c							\
					encoder/init.c								\
					encoder/fill.c								\
					encoder/pcode.c								\
					parser/parse.c								\
					parser/build_ast.c							\
					parser/is_valid.c							\
					parser/AST/add.c							\
					parser/AST/new.c							\
					parser/AST/free.c							\
					parser/AST/dump.c							\
					parser/AST/type.c							\
					parser/error/add.c							\
					parser/error/print.c						\
					parser/lexer/split_lines.c					\
					parser/lexer/tokenizer.c					\
					parser/lexer/free.c							\
					parser/lexer/dump.c							\
					parser/lexer/token_type/type.c				\
					parser/lexer/token_type/is_string.c			\
					parser/lexer/token_type/is_number.c			\
					parser/lexer/token_type/is_word.c			\
					parser/lexer/token_type/is_label.c			\
					parser/lexer/token_type/is_register.c		\
					parser/lexer/token_type/is_instruction.c	\
					parser/lexer/token_type/is_header.c			\
					parser/lexer/token_type/is_whitespace.c		\


SRC_ASM			:=	$(addprefix $(SRC_PATH)/asm/, $(BASENAME_ASM))
OBJ_ASM			:=	$(addprefix $(OBJ_PATH)/asm/, $(BASENAME_ASM:.c=.o))
DEP_ASM			:=	$(addprefix $(OBJ_PATH)/asm/, $(BASENAME_ASM:.c=.d))
