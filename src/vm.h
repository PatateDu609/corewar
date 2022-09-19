#ifndef VM_H
# define VM_H

typedef struct	s_vm
{
	int	nbr_cycles;
	int	nbr_champions;
        int	nchampion[MAX_PLAYERS];
	char	champion[MAX_PLAYERS];
	char	arena[MEM_SIZE];
}		t_vm;


#endif