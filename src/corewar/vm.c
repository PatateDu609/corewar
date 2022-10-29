#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include "vm.h"

// #include "../lib/libft/include/libft.h"

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
		int pos = PROG_NAME_LENGTH + COMMENT_LENGTH + 16;
		vars->champion[i].inst_len = vars->champion[i].length - pos;
		int j = 0;
		if (!(vars->champion[i].instructions = (unsigned char *)malloc(sizeof(char) * vars->champion[i].inst_len)))
			return;
		while (pos < vars->champion->length)
		{
			vars->champion[i].instructions[j] = vars->champion[i].file[pos];
			pos++;
			j++;
		}
		for (int k = 0; k < vars->champion[i].inst_len; k++) // display champ instructions for debug
			printf("%d ", vars->champion[i].instructions[k]);
	}
	// now adding inst to area
}

void	load_champ(t_vm *vars)
{
	// t_op *op;
	// if (!(op = (t_op*)malloc(sizeof(t_op) * 100)))
	// 	return;
	char **code;
	if (!(code = (char **)malloc(sizeof(char *) * 200)))
		return;
	int j = 0;
	for (int i = 0; i < vars->nbr_champions; i++)
	{
		for (int k = 0; k < vars->champion[i].inst_len; k++)
		{
			// op[j].name = op_tab[vars->champion[i].instructions[k]].name;
			// op[j].nb_params = op_tab[vars->champion[i].instructions[k]].nb_params;
			// op[j].opcode = op_tab[vars->champion[i].instructions[k]].opcode;
			// op[j].nb_cycles = op_tab[vars->champion[i].instructions[k]].nb_cycles;
			// op[j].description = op_tab[vars->champion[i].instructions[k]].description;
			// op[j].has_pcode = op_tab[vars->champion[i].instructions[k]].has_pcode;
			// op[j].has_idx = op_tab[vars->champion[i].instructions[k]].has_idx;
			// op[j].param_types = op_tab[vars->champion[i].instructions[k]].param_types;
			// printf("\nop[%d] = { %s, %d, %d, %d, %s, %d, %d, ??? } \n", 0, op[j].name, op[j].nb_params, op[j].opcode, op[j].nb_cycles, op[j].description, op[j].has_pcode, op[j].has_idx);
			// code[j] = op_tab[vars->champion[i].instructions[k]].name;

			// strcat(code[j], " ");
			// printf("\ncode[j] = %s", code[j]);
			// j++;
			return;
		}
	}
}

void cycles_emulate(t_vm *vars)
{
	unsigned int **cycle;
	if (!(cycle = (unsigned int **)malloc(sizeof(unsigned int *) * 500)))
		return;

	int i = 0;
	while(i < vars->champion[0].inst_len)
	{
		// cycle[0][op_tab[vars->champion[0].instructions[i]].nb_cycles] = vars->champion[0].instructions[i];
		cycle[0][op_tab[vars->champion[0].instructions[i]].nb_cycles] = i;

		if (vars->champion[0].instructions[i] == 1) // "live"
			i += REG_SIZE; // ✅
		else if (vars->champion[0].instructions[i] == 2) // "ld"
			i += REG_SIZE + 2; // ✅
		else if (vars->champion[0].instructions[i] == 3) // "st"
			i += REG_SIZE; // verif + need exemple
		else if (vars->champion[0].instructions[i] == 4) // "add"
			i += REG_SIZE; // ✅
		else if (vars->champion[0].instructions[i] == 5) // "sub"
			i += REG_SIZE; // ✅
		else if (vars->champion[0].instructions[i] == 6) // "and"
			i += REG_SIZE; // ✅
		else if (vars->champion[0].instructions[i] == 7) // "or"
			i += REG_SIZE; // ✅
		else if (vars->champion[0].instructions[i] == 8) // "xor"
			i += REG_SIZE; // ✅
		else if (vars->champion[0].instructions[i] == 9) // "zjmp"
			i += 2;  // ✅
		else if (vars->champion[0].instructions[i] == 10) // "ldi"
			i += 6;  // ✅
		else if (vars->champion[0].instructions[i] == 11) // "sti"
			i += REG_SIZE; // idk
		else if (vars->champion[0].instructions[i] == 12) // "fork"
			i += REG_SIZE; // idk
		else if (vars->champion[0].instructions[i] == 13) // "lld"
			i += REG_SIZE + 2; // ✅
		else if (vars->champion[0].instructions[i] == 14) // lldi
			i += 6; // ✅
		else if (vars->champion[0].instructions[i] == 15) // lfork
			i += REG_SIZE; // idk
		else if (vars->champion[0].instructions[i] == 16) // "aff"
			i += REG_SIZE; // idk

		// if (i > MEM_SIZE) // demander !!!!
		// 	i = 0;
	}

	load_arena(&vars, cycle);
}

void load_arena(t_vm *vars, unsigned int **cycle)
{
	unsigned int **r;
	if (!(r = (unsigned int **)malloc(sizeof(unsigned int *) * 16)))
		return;

	int i = 0;

	for (int j = 0; cycle[0][j]; j++)
	{
		i = cycle[0][j];
		if (vars->champion[0].instructions[i] == 1) // "live"
		{
			printf("live %c\n", vars->champion[0].instructions[i  + REG_SIZE]);
			// i += REG_SIZE; // REG_SIZE = 4 by default
		}
		else if (vars->champion[0].instructions[i] == 2) // "load"
		{
			printf("ld\n");
			r[vars->champion[0].instructions[i + REG_SIZE + 2]][0] = (i + (vars->champion[0].instructions[REG_SIZE + 1] % IDX_MOD));
			// RX = (i + (? % IDX_MOD))
			// i += REG_SIZE + 2;
		}
		else if (vars->champion[0].instructions[i] == 3) // "store"
		{
			printf("st\n");
			// need example !!!
			// if (r,r); r2 = r1
			// else (PC + (2nd arg % IDX_MOD)) = r1
		}
		else if (vars->champion[0].instructions[i] == 4) // "add"
		{
			r[vars->champion[0].instructions[i + 4]][0] = r[vars->champion[0].instructions[i + 2]][0] + r[vars->champion[0].instructions[i + 3]][0];
			printf("add %c %c %c\n", vars->champion[0].instructions[i + 2], vars->champion[0].instructions[i + 3], vars->champion[0].instructions[i + 4]);
			// i += REG_SIZE;
		}
		else if (vars->champion[0].instructions[i] == 5) // "sub"
		{
			r[vars->champion[0].instructions[i + 4]][0] = r[vars->champion[0].instructions[i + 2]][0] - r[vars->champion[0].instructions[i + 3]][0];
			printf("sub %c %c %c\n", vars->champion[0].instructions[i + 2], vars->champion[0].instructions[i + 3], vars->champion[0].instructions[i + 4]);
			// i += REG_SIZE;
		}
		else if (vars->champion[0].instructions[i] == 6) // "and"
		{
			r[vars->champion[0].instructions[i + 4]][0] = r[vars->champion[0].instructions[i + 2]][0] & r[vars->champion[0].instructions[i + 3]][0];
			printf("and %c %c %c\n", vars->champion[0].instructions[i + 2], vars->champion[0].instructions[i + 3], vars->champion[0].instructions[i + 4]);
			// i += REG_SIZE;
		}
		else if (vars->champion[0].instructions[i] == 7) // "or"
		{
			r[vars->champion[0].instructions[i + 4]][0] = r[vars->champion[0].instructions[i + 2]][0] | r[vars->champion[0].instructions[i + 3]][0];
			printf("or %c %c %c\n", vars->champion[0].instructions[i + 2], vars->champion[0].instructions[i + 3], vars->champion[0].instructions[i + 4]);
			// i += REG_SIZE;
		}
		else if (vars->champion[0].instructions[i] == 8) // "xor"
		{
			r[vars->champion[0].instructions[i + 4]][0] = r[vars->champion[0].instructions[i + 2]][0] ^ r[vars->champion[0].instructions[i + 3]][0];
			printf("xor %c %c %c\n", vars->champion[0].instructions[i + 2], vars->champion[0].instructions[i + 3], vars->champion[0].instructions[i + 4]);
			// i += REG_SIZE;
		}
		else if (vars->champion[0].instructions[i] == 9) // "zjmp"
		{
			printf("zjmp %c %c\n", vars->champion[0].instructions[i + 1], vars->champion[0].instructions[i + 2]);
			if (vars->champion[0].instructions[i + 1] == 1)
			{
				i = (i + (vars->champion[0].instructions[i + 2] % IDX_MOD));
				// PC = (PC + (? % IDX_MOD))
				// !!! do not forget cycles !!!
			}
			// else
				// consumes cycles
			// i += 2;
		}
		else if (vars->champion[0].instructions[i] == 10) // "ldi"
		{
			printf("ldi\n");
			unsigned int S = (i + (vars->champion[0].instructions[i + 3] % IDX_MOD)) + (vars->champion[0].instructions[i + 4] + vars->champion[0].instructions[i + 5]);
			// S = (PC + (1ST PARAM % IDX_MOD)) + 2ND PARAM
			r[vars->champion[0].instructions[i + 6]][0] = (i + (S % IDX_MOD));
			// R = (PC + (S % IDX_MOD))
			// i += 6;
		}
		else if (vars->champion[0].instructions[i] == 11) // "sti"
		{
			printf("sti\n");
			// DEMANDER CONFIRMATION
			// arena[PARAM2 + PARAM3] = PARAM1
			// i += ?;
		}
		else if (vars->champion[0].instructions[i] == 12) // "fork"
		{
			printf("fork\n");
			// new program start at = (PC + (first parameter % IDX_MOD))
			// PC += ?;
		}
		else if (vars->champion[0].instructions[i] == 13) // "lld"
		{
			printf("lld\n");
			r[vars->champion[0].instructions[i + REG_SIZE + 2]][0] = (i + (vars->champion[0].instructions[REG_SIZE + 1]));
			// RX = (PC + (?))
			//  modify the carry ????
			// i += REG_SIZE + 2;
		}
		else if (vars->champion[0].instructions[i] == 14) // "lldi"
		{
			printf("lldi\n");
			unsigned int S = (i + (vars->champion[0].instructions[i + 3])) + (vars->champion[0].instructions[i + 4] + vars->champion[0].instructions[i + 5]);
			// S = (PC + (1ST PARAM)) + 2ND PARAM
			r[vars->champion[0].instructions[i + 6]][0] = (i + (S));
			// R = (PC + (S))
			//  modify the carry ????
			// i += 6;
		}
		else if (vars->champion[0].instructions[i] == 15) // "lfork"
		{
			printf("lfork\n");
			// new program start at = (PC + (first parameter))
			// PC += ?;
		}
		else if (vars->champion[0].instructions[i] == 16) // "aff"
		{
			printf("aff\n");
			r[vars->champion[0].instructions[i  + REG_SIZE + 1]] = vars->champion[0].instructions[i  + REG_SIZE];
			printf("%c", (char)r[vars->champion[0].instructions[i  + REG_SIZE + 1]] % 256);
			// i += 4; // verif !!!
		}
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
			printf("Error: bad COREWAR_EXEC_MAGIC: %x != %x\n", vars->champion[i].header->magic, COREWAR_EXEC_MAGIC);
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
	load_champ(&vars);
	free_all(&vars);
	return (0);
}
