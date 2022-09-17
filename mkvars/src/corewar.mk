BASENAME_COREWAR	:=

SRC_COREWAR			:=	$(addprefix $(SRC_PATH)/corewar/, $(BASENAME_COREWAR))
OBJ_COREWAR			:=	$(addprefix $(OBJ_PATH)/corewar/, $(BASENAME_COREWAR:.c=.o))
DEP_COREWAR			:=	$(addprefix $(OBJ_PATH)/corewar/, $(BASENAME_COREWAR:.c=.d))
