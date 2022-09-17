RESET_COLOR			:=	\e[0m

RED					:=	\e[38;5;124m
GREEN				:=	\e[38;5;46m
BLUE				:=	\e[38;5;21m

BOLD				:=	\e[1m

INFO				:=	$(BLUE)$(BOLD)≫$(RESET)$(BLUE)
ERROR				:=	$(RED)$(BOLD)≫$(RESET)$(RED)
DONE				:=	$(GREEN)$(BOLD)≫$(RESET)$(GREEN)
