#ifndef VM_H
# define VM_H

#include "op.h"

#define TOTAL_SIZE (CHAMP_MAX_SIZE + PROG_NAME_LENGTH + COMMENT_LENGTH + 1)

typedef struct	s_vm
{
	int	nbr_cycles;
	int	nbr_champions;
        int	nchampion[MAX_PLAYERS];
	char*	champion[MAX_PLAYERS];
	char*	champion_string[MAX_PLAYERS];
	int	champion_len[MAX_PLAYERS];
	char	arena[MEM_SIZE];
}		t_vm;

// typedef struct s_arena
// {
// 	char	byte[MEM_SIZE];
// }		t_arena;

// To-do list:
// 	1) réussir à parser un .cor (Error: read)
// 	2) refaire une belle structure
// 	3) pk je n'arrive pas à utiliser la libft ????

#endif