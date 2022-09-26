#include <stdio.h>
#include <string.h>
#include "vm.h"
#include <fcntl.h>
#include "../lib/libft/include/libft.h"

void	init_vm(t_vm *vars)
{
	vars->nbr_cycles = 0;
	vars->nbr_champions = 0;
}

int	parsing_args(int argc, char **argv, t_vm *vars)
{
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-dump") == 0)
		{
			vars->nbr_cycles = atoi(argv[i + 1]);
		}
		else if (strcmp(argv[i], "-n") == 0)
		{
			vars->nchampion[vars->nbr_champions] = atoi(argv[i + 1]);
			vars->champion[vars->nbr_champions] = argv[i + 2];
			vars->nbr_champions++;
			i++;
		}
		else
		{
			vars->nchampion[vars->nbr_champions] = vars->nbr_champions;
			vars->champion[vars->nbr_champions] = argv[i];
			// printf("Champ n%d: %s !\n",vars->nbr_champions, vars->champion[vars->nbr_champions]);
			vars->nbr_champions++;
		}
	}
	return (1);
}

int	string_len(t_vm *vars, int i)
{
	char	buff[BUFF_SIZE + 1];
	int	ret;
	int	fd;

	vars->champion_len[i] = 0;
	if ((fd = open(vars->champion[i], O_RDONLY)) == -1)
		printf("Error: open file %s\n", vars->champion[i]);
	while ((ret = read(fd, buff, BUFF_SIZE)) > 0)
	{
		if (ret != 0)
			vars->champion_len[i] += ret;
	}
	if (vars->champion_len[i] > TOTAL_SIZE)
		printf("Error: size file too big\n");
	if (close(fd) == -1)
		printf("Error: closing fd\n");
	if (ret == -1)
		printf("Error: idk\n");
	// printf("len = %d\n", vars->champion_len[i]);
	return (vars->champion_len[i]);
}

void	parsing_champ(t_vm *vars)
{
	int fd;
	int len;

	for (int i = 0; i < vars->nbr_champions; i++)
	{
		if (strcmp(vars->champion[i] + strlen(vars->champion[i]) - 4, ".cor") != 0)
			printf("Error: bad champ extension (%s) !\n", vars->champion[i]);
		len = string_len(vars, i);
		if ((fd = open(vars->champion[i], O_RDONLY)) == -1)
			printf("Error: open file %s\n", vars->champion[i]);
		if (read(fd, vars->champion_string[i], vars->champion_len[i]) == -1)
			printf("Error: read\n");
		vars->champion_string[i][len] = '\0'; // BUS ERROR ICI !!!
		printf("%s = %s\n", vars->champion[i], vars->champion_string[i]);
		if (close(fd) == -1)
			printf("Error: close\n");
	}
}

int	main(int argc, char**argv)
{
	t_vm vars;
	if (argc == 1)
		printf("Usage : corewar [-ctmo cycle_tmp_mod_out_val]\n\t[-comment] [-pourcent pourcent]\n\t[-by_step] [-aff] [-aff_shell] [-aff_proc]\n\t[-dump nbr_cycle] [-noX cycle_wait_before_affiche]\n\t[[-n prog_number] [-a load_address] prog_name] ...\n\t  Touches :\n\t\t+/- : ralenti\n\t\td   : dump mem\n\t\ts   : step by step On/Off ('space'== next cycle)\n\t\tp   : aff_shell On/Off\n\t\ta   : aff On/Off\n\t\tq   : exit");
	// if (argc > CHAMP_MAX_SIZE * 3 + 2 || argc < 2 + 2 * 3)
	// 	printf("Error: bad number of args\n");
	init_vm(&vars);
	if (!parsing_args(argc, argv, &vars))
		printf("Error: bad args\n");
	parsing_champ(&vars);
	return (0);
}