# Add there all common sources (parsing and loading for example...)

BASENAME	:=	main.c

SRC			:=	$(addprefix $(SRC_PATH)/, $(BASENAME))
OBJ			:=	$(addprefix $(OBJ_PATH)/, $(BASENAME:.c=.o))
DEP			:=	$(addprefix $(OBJ_PATH)/, $(BASENAME:.c=.d))
