#include <stdio.h>
#include <string.h>
#include "vm.h"
#include <fcntl.h>
#include "../lib/libft/include/libft.h"

void	free_all(t_vm *vars)
{
	for (int i = 0; i < vars->nbr_champions; i++)
		free(vars->champion[i].file);
}

void	exit_vm(t_vm *vars)
{
	free_all(vars);
	exit(0);
}

void	init_vm(t_vm *vars)
{
	vars->nbr_cycles = 0;
	vars->nbr_champions = 0;
	vars->champion->live = 0;
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
			vars->champion[vars->nbr_champions].number = atoi(argv[i + 1]);
			vars->champion[vars->nbr_champions].path = argv[i + 2];
			vars->nbr_champions++;
			i++;
		}
		else
		{
			vars->champion[vars->nbr_champions].number = vars->nbr_champions;
			vars->champion[vars->nbr_champions].path = argv[i];
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

	vars->champion[i].length = 0;
	if ((fd = open(vars->champion[i].path, O_RDONLY)) == -1)
		printf("Error: open file %s\n", vars->champion[i].path);
	while ((ret = read(fd, buff, BUFF_SIZE)) > 0)
	{
		if (ret != 0)
			vars->champion[i].length += ret;
	}
	if (!(vars->champion[i].file = (unsigned char*)malloc(sizeof(char) * vars->champion[i].length)))
		return (-1);
	if (close(fd) == -1) 
		printf("Error: closing fd\n");
	if (ret == -1)
		printf("Error: idk\n");
	return (vars->champion[i].length);
}

void	parsing_champ(t_vm *vars)
{
	int fd;
	int len;
	int test;

	for (int i = 0; i < vars->nbr_champions; i++)
	{
		if (strcmp(vars->champion[i].path + strlen(vars->champion[i].path) - 4, ".cor") != 0)
		{
			printf("Error: bad champ extension (%s) !\n", vars->champion[i].path);
			exit_vm(vars);
		}
		if ((len = string_len(vars, i)) == -1)
		{
			printf("Error: file len %s\n", vars->champion[i].path);
			exit_vm(vars);
		}
		if ((fd = open(vars->champion[i].path, O_RDONLY)) == -1)
		{
			printf("Error: open file %s\n", vars->champion[i].path);
			exit_vm(vars);
		}
		if (read(fd, vars->champion[i].file, vars->champion[i].length) == -1)
		{
			printf("Error: read %d\n", i);
			exit_vm(vars);
		}
		if (close(fd) == -1)
		{
			printf("Error: close (parsing_champ)\n");
			exit_vm(vars);
		}
		// for (int j = 0; j < vars->champion[i].length; j++) // display champ code for debug
		// 	printf("0x%.2x ", vars->champion[i].file[j]);
	}
}

void	champ_enter_arena(t_vm *vars)
{
	if (!(vars->arena = (char *)malloc(sizeof(char) * MEM_SIZE)))
		return;
	// get champ instructions
	for (int i = 0; i < vars->nbr_champions; i++)
	{
		int pos = PROG_NAME_LENGTH + COMMENT_LENGTH;
		vars->champion[i].inst_len = vars->champion[i].length - pos;
		int j = 0;
		if (!(vars->champion[i].instructions = (unsigned char *)malloc(sizeof(char) * vars->champion[i].inst_len)))
			return;
		while (pos < vars->champion->length)
		{
			vars->champion[i].instructions[j] = vars->champion[i].file[pos];
			pos++;
		}
		for (int k = 0; k < vars->champion[i].inst_len; k++) // display champ instructions for debug
			printf("0x%.2x ", vars->champion[i].instructions[k]);
	}
}

void	get_champ_info(t_vm *vars)
{
	t_header *infos;

	for (int i = 0; i < vars->nbr_champions; i++)
	{
		infos = (t_header *)vars->champion[i].file;
		// printf("magic = %d\nprog_name = %s\nprog_size = %d\ncomment = %s",
		// 	infos->magic, infos->prog_name, infos->prog_size, infos->comment);
		vars->champion[i].header = infos;
		// printf("\nmagic = %d\nprog_name = %s\nprog_size = %d\ncomment = %s",
		// 	vars->champion[i].header->magic, vars->champion[i].header->prog_name, 
		// 	vars->champion[i].header->prog_size, vars->champion[i].header->comment);
		// Checking infos
		if (vars->champion[i].header->magic != COREWAR_EXEC_MAGIC)
			printf("Error: bad COREWAR_EXEC_MAGIC: %d != %d\n", vars->champion[i].header->magic, COREWAR_EXEC_MAGIC);
		if (strlen(vars->champion[i].header->prog_name) > PROG_NAME_LENGTH)
			printf("Error: bad PROG_NAME_LENGTH: %lu != %d\n", strlen(vars->champion[i].header->prog_name), PROG_NAME_LENGTH);
		if (strlen(vars->champion[i].header->comment) > COMMENT_LENGTH)
			printf("Error: bad COMMENT_LENGTH: %lu != %d\n", strlen(vars->champion[i].header->comment), COMMENT_LENGTH);
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
	get_champ_info(&vars);
	champ_enter_arena(&vars);
	free_all(&vars);
	return (0);
}
