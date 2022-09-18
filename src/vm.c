#include <stdio.h>
#include "op.h"
#include "vm.h"

void	init_vm(t_vm vars)
{
	vars->nbr_cycles = 0;
	vars->nbr_champions = 0;
}

int	parsing_args(int argc, char **argv)
{
	for (int i = 1; i < argc; i++)
	{
		if (*argv[i] == "-dump")
		{
			nbr_cycles = argv[i + 1];
		}
		else if (*argv[i] == "-n")
		{
			nbr_champions++;
			nchampion = atoi(*argv[i + 1]);
			champion = *argv[i + 2];
			i++;
		}
		else
		{
			nbr_champions++;
			nchampion = nbr_champions;
			champion = *argv[i];
		}
	}
	return (1);
}

int	main(int argc, char**argv)
{
	t_vm vars;
	if (argc > CHAMP_MAX_SIZE * 3 + 2 || argc < 2 + 2 * 3) // Reverifier !!!
		printf("Error: bad number of args\n");
	init_vm(&vars);
	if (!parsing_args(argc, argv))
		printf("Error: bad args\n");
	
	return (0);
}