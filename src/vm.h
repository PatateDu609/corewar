#ifndef VM_H
# define VM_H

#include "op.h"

#define TOTAL_SIZE (CHAMP_MAX_SIZE + PROG_NAME_LENGTH + COMMENT_LENGTH + 1)

typedef struct	s_champ
{
	char*		path;
	int		number;
	char*		name;
	char*		comment;
	int		length;
	unsigned char*	file;
	t_header*	header;
}		t_champ;

typedef struct	s_vm
{
	int	nbr_cycles;
	int	nbr_champions;
	t_champ	champion[MAX_PLAYERS];
	char	arena[MEM_SIZE];
}		t_vm;

// typedef struct s_arena
// {
// 	char	byte[MEM_SIZE];
// }		t_arena;


#endif