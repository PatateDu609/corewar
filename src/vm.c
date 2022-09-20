#include <stdio.h>
#include "op.h"
#include "vm.h"

void	init_vm(t_vm *vars)
{
	vars->nbr_cycles = 0;
	vars->nbr_champions = 0;
}

int	parsing_args(int argc, char **argv, t_vm *vars)
{
	for (int i = 1; i < argc; i++)
	{
		if (*argv[i] == "-dump")
		{
			vars->nbr_cycles = argv[i + 1];
		}
		else if (*argv[i] == "-n")
		{
			vars->nbr_champions++;
			vars->nchampion[vars->nbr_champions] = atoi(*argv[i + 1]);
			vars->champion[vars->nbr_champions] = *argv[i + 2];
			i++;
		}
		else
		{
			vars->nbr_champions++;
			vars->nchampion[vars->nbr_champions] = vars->nbr_champions;
			vars->champion[vars->nbr_champions] = *argv[i];
		}
	}
	return (1);
}

void	parsing_files(t_vm *vars)
{
	for (int i = 0; i < vars->nbr_champions; i++)
	{
		
	}
}

int	main(int argc, char**argv)
{
	t_vm vars;
	if (argc > CHAMP_MAX_SIZE * 3 + 2 || argc < 2 + 2 * 3) // Reverifier !!!
		printf("Error: bad number of args\n");
	init_vm(&vars);
	if (!parsing_args(argc, argv, &vars))
		printf("Error: bad args\n");
	parsing_files(&vars);
	return (0);
}