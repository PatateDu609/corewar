NAME_ASM		=	asm
NAME_COREWAR	=	corewar

AUTHORS			=	gboucett rbourgea

# Comment this line to disable the debug mode
DEBUG			=	1

CC				:=	gcc
CFLAGS			:=	-Wall -Werror -Wextra

ifdef DEBUG
CFLAGS			+=	-g3 -ggdb -O0
else
CFLAGS			+=	-O2
endif

SRC_PATH		:=	src
OBJ_PATH		:=	obj
INC_PATH		:=	include
LIB_PATH		:=	lib

LIBFT_PATH		:=	$(LIB_PATH)/libft
LIBFT			:=	$(LIBFT_PATH)/libft.a
LIBSTC42_PATH	:=	$(LIB_PATH)/libstc42

# Maybe we'll need the libft... or another homemade library
CFLAGS			+=	-I$(INC_PATH) -I$(LIBFT_PATH)/include -I$(LIBSTC42_PATH)/include
LDFLAGS			:= -L$(LIBFT_PATH) -lft
