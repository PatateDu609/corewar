/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbourgea <rbourgea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 14:40:08 by rbourgea          #+#    #+#             */
/*   Updated: 2022/11/17 14:44:44 by rbourgea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

static int (*const op_func[])(t_stat*, t_champion*) = { &nothing, &live, &ld, &st, &add, &sub, &and, &or, &xor, &zjump, &ldi, &sti, &ft_fork, &lld, &lldi, &lfork, &aff };

int	ft_atoi(const char *str)
{
	int	res;
	int	neg;

	neg = 1;
	res = 0;
	while (*str && (*str == ' ' || *str == '\n' || *str == '\t' ||
					*str == '\v' || *str == '\f' || *str == '\r'))
		++str;
	if (*str == '-')
		neg = -1;
	if (*str == '-' || *str == '+')
		++str;
	while (*str && *str >= '0' && *str <= '9')
	{
		res = res * 10 + (*str - 48);
		++str;
	}
	return (res * neg);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s2)
	{
		if (*s1 != *s2)
			return (*s1 - *s2);
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}

int	ft_strlen(const char *str)
{
	int	len;

	len = 0;
	while (str[len])
		len++;
	return (len);
}

int nothing(t_stat *stats, t_champion *champ)
{
	return 0;
}

/* 0x01 LIVE ********************************************************** */
/* Followed by 4 bytes representing the player name. This instruction   */
/* indicates that the player is alive. (No parameter encoding byte).    */
/* ******************************************************************** */

int live(t_stat *stats, t_champion *champ)
{
	champ->pc = (champ->pc + 1) % MEM_SIZE;
	printf("The player %i (%s) is alive.\n", champ->number, champ->header.prog_name);
	champ->pc = (champ->pc + DIR_SIZE) % MEM_SIZE;
	champ->is_alive = TRUE;
	stats->nb_live += 1;
	stats->last_user_live = champ->number;
	return 0;
}

/* 0x02 LD ************************************************************ */
/* This instruction takes 2 parameters, the 2nd of which has to be a    */
/* register (not the PC) It loads the value of the first parameter in   */
/* the register. This operation modifies the carry. ld 34,r3 loads the  */
/* REG_SIZE bytes from address (PC + (34 % IDX_MOD)) in                 */
/* register r3.                                                         */
/* ******************************************************************** */

int ld(t_stat *stats, t_champion *champ)
{
	char code;
	int ind_or_dir;
	code = (champ->pc + 1) % MEM_SIZE;
	champ->pc = (champ->pc + 2) % MEM_SIZE;
	ind_or_dir = get_type(code, champ, stats);
	if (INDIRECT(code) == 1)
	{
		if (stats->arena.zone[champ->pc] <= REG_NUMBER)
			champ->registers[stats->arena.zone[champ->pc]] = stats->arena.zone[(champ->pc + ind_or_dir % IDX_MOD) % MEM_SIZE];
	}
	else if (DIRECT(code) == 1)
		if (stats->arena.zone[champ->pc] <= REG_NUMBER)
			champ->registers[stats->arena.zone[champ->pc]] = ind_or_dir % IDX_MOD;
	champ->pc = (champ->pc + 1) % MEM_SIZE;
	return 0;
}

/* 0x03 ST ************************************************************ */
/* This instruction takes 2 parameters. It stores (REG_SIZE bytes)      */
/* the value of the first argument (always a register) in the second.   */
/* st r4,34 stores the value of r4 at the address (PC + (34 %           */
/* IDX_MOD)) st r3,r8 copies r3 in r8                                   */
/* ******************************************************************** */

int st(t_stat *stats, t_champion *champ)
{
	char code;
	int ret = 0;
	int res = 0;
	char nb[4];
	code = stats->arena.zone[champ->pc + 1 % MEM_SIZE] << 2;
	if (stats->arena.zone[champ->pc + 2 % MEM_SIZE] <= REG_NUMBER)
		ret = champ->registers[stats->arena.zone[champ->pc + 2 % MEM_SIZE]];
	if (DIRECT(code) == 1)
	{
		for (int i = 0; i < 4; i++)
			nb[i] = stats->arena.zone[champ->pc + 3 + i % MEM_SIZE];
		res = SWAP_ENDIAN(*(int*)nb);
		stats->arena.zone[champ->pc + res % IDX_MOD] = ret;
		champ->pc = (champ->pc + 3 + DIR_SIZE) % MEM_SIZE;
	} 
	else if (REGISTER(code) == 1)
	{
		champ->registers[stats->arena.zone[champ->pc + 3 % MEM_SIZE]] = ret;
		champ->pc = (champ->pc + 4) % MEM_SIZE;
	}
	return 0;
}

/* 0x04 ADD *********************************************************** */
/* This instruction takes 3 registers as parameter, adds the contents   */
/* of the 2 first and stores the result in the third. This operation    */
/* modifies the carry. add r2,r3,r5 adds r2 and r3 and stores the       */
/* result in r5                                                         */
/* ******************************************************************** */

int add(t_stat *stats, t_champion *champ)
{
	champ->pc = (champ->pc + 2) % MEM_SIZE;
	if (stats->arena.zone[champ->pc] <= REG_NUMBER && stats->arena.zone[champ->pc + 1] <= REG_NUMBER && stats->arena.zone[champ->pc + 2] <= REG_NUMBER)
	{
		champ->registers[stats->arena.zone[champ->pc + 2]] = champ->registers[stats->arena.zone[champ->pc + 1]] + champ->registers[stats->arena.zone[champ->pc]];
		champ->pc = (champ->pc + 3) % MEM_SIZE;	
	}
	return 0;
}

/* 0x05 SUB *********************************************************** */
/* This instruction takes 3 registers as parameter, adds the contents   */
/* of the 2 first and stores the result in the third. This operation    */
/* modifies the carry. add r2,r3,r5 subs r2 and r3 and stores the       */
/* result in r5                                                         */
/* ******************************************************************** */

int sub(t_stat *stats, t_champion *champ)
{
	champ->pc = (champ->pc + 2) % MEM_SIZE;
	if (stats->arena.zone[champ->pc] <= REG_NUMBER && stats->arena.zone[champ->pc + 1] <= REG_NUMBER && stats->arena.zone[champ->pc + 2] <= REG_NUMBER)
		champ->registers[stats->arena.zone[champ->pc + 2]] = champ->registers[stats->arena.zone[champ->pc]] - champ->registers[stats->arena.zone[champ->pc + 1]];
	champ->pc = (champ->pc + 3) % MEM_SIZE;
	return 0;
}

/* 0x06 AND *********************************************************** */
/* p1 & p2 -> p3, the parameter 3 is always a register This             */
/* operation modifies the carry. and r2, %0,r3 stores r2 & 0 in r3.     */
/* ******************************************************************** */

int get_type(char code, t_champion *champ, t_stat *stats)
{
	int res = 0;
	char nb[4];
	for (int i = 0; i < 4; i++)
		nb[i] = 0;
	if (INDIRECT(code) == 1)
	{
		nb[0] = stats->arena.zone[champ->pc % MEM_SIZE];
		nb[1] = stats->arena.zone[champ->pc + 1 % MEM_SIZE];
		res = SWAP_ENDIAN_16(*(int*)nb);
		res = stats->arena.zone[champ->pc + res];
		champ->pc = (champ->pc + IND_SIZE) % MEM_SIZE;
	}
	else if (DIRECT(code) == 1)
	{
		for (int i = 0; i < 4; i++)
			nb[i] = stats->arena.zone[champ->pc + i % MEM_SIZE];
		res = SWAP_ENDIAN(*(int*)nb);
		champ->pc = (champ->pc + DIR_SIZE) % MEM_SIZE;
	} 
	else if (REGISTER(code) == 1)
	{
		res = champ->registers[stats->arena.zone[champ->pc]];
		champ->pc = (champ->pc + 1) % MEM_SIZE;
	}
	return res;
}

int and(t_stat *stats, t_champion *champ)
{
	char code;
	char *bin;
	int nb1 = 0;
	int nb2 = 0;
	code = (champ->pc + 1) % MEM_SIZE;
	champ->pc = (champ->pc + 2) % MEM_SIZE;
	nb1 = get_type(code, champ, stats);
	nb2 = get_type(code << 2, champ, stats);
	if (stats->arena.zone[champ->pc] <= REG_NUMBER)
		champ->registers[stats->arena.zone[champ->pc]] = nb1 & nb2;
	champ->pc = (champ->pc + 1) % MEM_SIZE;
	return 0;
}

/* 0x07 OR ************************************************************ */
/* Same as and but with an or (| in C)                                  */
/* ******************************************************************** */

int or(t_stat *stats, t_champion *champ)
{
	char code;
	char *bin;
	int nb1 = 0;
	int nb2 = 0;
	code = (champ->pc + 1) % MEM_SIZE;
	champ->pc = (champ->pc + 2) % MEM_SIZE;
	nb1 = get_type(code, champ, stats);
	nb2 = get_type(code << 2, champ, stats);
	if (stats->arena.zone[champ->pc] <= REG_NUMBER)
		champ->registers[stats->arena.zone[champ->pc]] = nb1 | nb2;
	champ->pc = (champ->pc + 1) % MEM_SIZE;
	return 0;
}

/* 0x08 XOR *********************************************************** */
/* Same as and but with an xor (în C)                                   */
/* ******************************************************************** */

int xor(t_stat *stats, t_champion *champ)
{
	char code;
	char *bin;
	int nb1 = 0;
	int nb2 = 0;
	code = (champ->pc + 1) % MEM_SIZE;
	champ->pc = (champ->pc + 2) % MEM_SIZE;
	nb1 = get_type(code, champ, stats);
	nb2 = get_type(code << 2, champ, stats);
	if (stats->arena.zone[champ->pc] <= REG_NUMBER)
		champ->registers[stats->arena.zone[champ->pc]] = nb1 ^ nb2;
	champ->pc = (champ->pc + 1) % MEM_SIZE;
	return 0;
}

/* 0x09 ZJUMP ********************************************************* */
/* This instruction is not followed by any parameter encoding byte.     */
/* It always takes an index (IND_SIZE) and makes a jump at this         */
/* index if the carry is set to 1. If the carry is null, zjmp does      */
/* nothing but consumes the same amount of time. zjmp %23 does :        */
/* If carry == 1, store (PC + (23 % IDX_MOD)) in the PC.                */
/* ******************************************************************** */

int zjump(t_stat *stats, t_champion *champ)
{
	int res = 0;
	char nb[4];
	nb[0] = stats->arena.zone[champ->pc + 1 % MEM_SIZE];
	nb[1] = stats->arena.zone[champ->pc + 2 % MEM_SIZE];
	nb[2] = 0;
	nb[3] = 0;
	res = SWAP_ENDIAN_16(*(int*)nb);
	champ->pc = (champ->pc + 3 + (res % IDX_MOD)) % MEM_SIZE;
	return 0;
}

/* 0x0a LDI *********************************************************** */
/* This operation modifies the carry. ldi 3,%4,r1 reads IND_SIZE        */
/* bytes at address: (PC + (3 % IDX_MOD)), adds 4 to this value.        */ 
/* We will name this sum S. Read REG_SIZE bytes at address (PC          */
/* + (S % IDX_MOD)), which are copied to r1. Parameters 1 and 2         */
/* are indexes.                                                         */
/* ******************************************************************** */

int ldi(t_stat *stats, t_champion *champ)
{
	int nb[4];
	int arg1;
	int arg2;
	int reg;
	for (int i = 0; i < 4; i++)
		nb[i] = 0;
	champ->pc = (champ->pc + 2) % MEM_SIZE;
	nb[0] = stats->arena.zone[champ->pc % MEM_SIZE];
	nb[1] = stats->arena.zone[champ->pc + 1 % MEM_SIZE];
	arg1 = SWAP_ENDIAN_16(*(int*)nb);
	champ->pc = (champ->pc + IND_SIZE) % MEM_SIZE;
	nb[0] = stats->arena.zone[champ->pc % MEM_SIZE];
	nb[1] = stats->arena.zone[champ->pc + 1 % MEM_SIZE];
	arg2 = SWAP_ENDIAN_16(*(int*)nb);
	champ->pc = (champ->pc + IND_SIZE) % MEM_SIZE;
	reg = champ->registers[stats->arena.zone[champ->pc]];
	champ->pc = (champ->pc + 1) % MEM_SIZE;
	arg1 = stats->arena.zone[(champ->pc + arg1) % IDX_MOD] + 4;
	champ->registers[reg] = (champ->pc + arg1) % IDX_MOD;
	return 0;
}

/* 0x0b STI *********************************************************** */
/* sti r2,%4,%5 sti copies REG_SIZE bytes of r2 at address (4 + 5)      */
/* Parameters 2 and 3 are indexes. If they are, in fact, registers,     */
/* we’ll use their contents as indexes.                                 */
/* ******************************************************************** */

int get_index_or_register(t_stat *stats, t_champion *champ, int code, int save)
{
	int res = 0;
	char nb[4];
	if (INDIRECT(code) == 1)
	{
		nb[0] = stats->arena.zone[champ->pc % MEM_SIZE];
		nb[1] = stats->arena.zone[champ->pc + 1 % MEM_SIZE];
		res = SWAP_ENDIAN_16(*(int*)nb);
		res = stats->arena.zone[save + res % IDX_MOD];
		champ->pc = (champ->pc + DIR_SIZE) % MEM_SIZE;
	}
	else if (REGISTER(code) == 1)
	{
		res = champ->registers[stats->arena.zone[champ->pc % MEM_SIZE]];
		champ->pc = (champ->pc + 1) % MEM_SIZE;
	}
	return res;
}

int sti(t_stat *stats, t_champion *champ)
{
	char code;
	int ret = 0;
	int nb = 0;
	int save_pc = champ->pc;
	code = stats->arena.zone[champ->pc + 1 % MEM_SIZE] << 2;
	champ->pc = (champ->pc + 2) % MEM_SIZE;
	if (stats->arena.zone[champ->pc % MEM_SIZE] <= REG_NUMBER)
		ret = champ->registers[stats->arena.zone[champ->pc % MEM_SIZE]];
	nb = get_index_or_register(stats, champ, code, save_pc);
	nb += get_index_or_register(stats, champ, code << 2, save_pc);
	stats->arena.zone[save_pc + nb % MEM_SIZE] = ret;
	return 0;
}

/* 0x0c FORK ********************************************************** */
/* This instruction is not followed by a parameter encoding byte. It    */
/* always takes an index and creates a new program, which is            */
/* executed from address : (PC + (first parameter % IDX_MOD)).          */
/* Fork %34 creates a new program. The new program inherits all of      */
/* its father’s states.                                                 */
/* ******************************************************************** */

t_champion copy_champ(t_champion *champ)
{
	t_champion tmp;
	tmp.number = champ->number;
	tmp.load_adress = champ->load_adress;
	for (int i = 0; i < REG_NUMBER + 1; i++)
		tmp.registers[i] = champ->registers[i];
	tmp.pc = champ->pc;
	tmp.carry = champ->carry;
	tmp.is_alive = champ->is_alive;
	tmp.living = champ->living;
	tmp.is_waiting = champ->is_waiting;
	tmp.code = champ->code;
	tmp.header = champ->header;
	return tmp;
}

int get_index(t_stat *stats, t_champion *champ)
{
	int i;

	for (int i = 0; i < stats->nb_champions; i++)
		if (stats->champions[i].number == champ->number)
			return i;
	return 0;
}

t_champion *realloc_champ(t_stat *stats, int index, int offset)
{
	int i;
	t_champion *tmp = malloc(sizeof(t_champion) * (stats->nb_champions + 1));
	for (i = 0; i <= index; i++)
		tmp[i] = copy_champ(&stats->champions[i]);
	tmp[i] = copy_champ(&stats->champions[i - 1]);
	for (int j = i + 1; j < (stats->nb_champions + 1); j++, i++)
		tmp[j] = copy_champ(&stats->champions[i]);
	tmp[index + 1].pc += offset % IDX_MOD;
	stats->nb_champions += 1;
	return tmp;
}

int ft_fork(t_stat *stats, t_champion *champ)
{
	char nb[4];
	champ->pc = (champ->pc + 1) % MEM_SIZE;
	nb[0] = stats->arena.zone[champ->pc];
	nb[1] = stats->arena.zone[champ->pc + 1];
	nb[2] = 0;
	nb[3] = 0;
	stats->champions = realloc_champ(stats, get_index(stats, champ),
	SWAP_ENDIAN_16(*(int*)nb));
	return 0;
}

/* 0x0d LLD ********************************************************** */
/* Same as ld, but without the % IDX_MOD This operation                */
/* modifies the carry.                                                 */
/* ******************************************************************* */

int lld(t_stat *stats, t_champion *champ)
{
    char code;
    int ind_or_dir;
    code = (champ->pc + 1) % MEM_SIZE;
    champ->pc = (champ->pc + 2) % MEM_SIZE;
    ind_or_dir = get_type(code, champ, stats);
    if (INDIRECT(code) == 1)
	{
		if (stats->arena.zone[champ->pc] <= REG_NUMBER)
            champ->registers[stats->arena.zone[champ->pc]] = stats->arena.zone[(champ->pc + ind_or_dir) % MEM_SIZE];
	}
    else if (DIRECT(code) == 1)
        if (stats->arena.zone[champ->pc] <= REG_NUMBER)
			champ->registers[stats->arena.zone[champ->pc]] = ind_or_dir;
    champ->pc = (champ->pc + 1) % MEM_SIZE;
    return 0;
}

/* 0x0e LLDI ********************************************************** */
/* Same as ldi, but without the % IDX_MOD This operation                */
/* modifies the carry.                                                  */
/* ******************************************************************** */

int lldi(t_stat *stats, t_champion *champ)
{
    int nb[4];
    int arg1;
    int arg2;
    int reg;
    for (int i = 0; i < 4; i++)
        nb[i] = 0;
    champ->pc = (champ->pc + 2) % MEM_SIZE;
    nb[0] = stats->arena.zone[champ->pc % MEM_SIZE];
    nb[1] = stats->arena.zone[champ->pc + 1 % MEM_SIZE];
    arg1 = SWAP_ENDIAN_16(*(int*)nb);
    champ->pc = (champ->pc + IND_SIZE) % MEM_SIZE;
    nb[0] = stats->arena.zone[champ->pc % MEM_SIZE];
    nb[1] = stats->arena.zone[champ->pc + 1 % MEM_SIZE];
    arg2 = SWAP_ENDIAN_16(*(int*)nb);
    champ->pc = (champ->pc + IND_SIZE) % MEM_SIZE;
    reg = champ->registers[stats->arena.zone[champ->pc]];
    champ->pc = (champ->pc + 1) % MEM_SIZE;
    arg1 = stats->arena.zone[(champ->pc + arg1)] + 4;
    champ->registers[reg] = (champ->pc + arg1);
    return 0;
}

/* 0x0f LFORK ********************************************************** */
/* Same as fork, but without the % IDX_MOD This operation                */
/* modifies the carry.                                                   */
/* ********************************************************************* */

t_champion *lrealloc_champ(t_stat *stats, int index, int offset)
{
    int i;
    t_champion *new = malloc(sizeof(t_champion) * (stats->nb_champions + 1));
    for (i = 0; i <= index; i++)
        new[i] = copy_champ(&stats->champions[i]);
    new[i] = copy_champ(&stats->champions[i - 1]);
    for (int j = i + 1; j < (stats->nb_champions + 1); j++, i++)
        new[j] = copy_champ(&stats->champions[i]);
    new[index + 1].pc += offset;
    stats->nb_champions += 1;
    return new;
}

int lfork(t_stat *stats, t_champion *champ)
{
    char nb[4];
    champ->pc = (champ->pc + 1) % MEM_SIZE;
    nb[0] = stats->arena.zone[champ->pc];
    nb[1] = stats->arena.zone[champ->pc + 1];
    nb[2] = 0;
    nb[3] = 0;
    stats->champions = lrealloc_champ(stats, get_index(stats, champ), SWAP_ENDIAN_16(*(int*)nb));
    return 0;
}

/* 0x10 AFF ************************************************************ */
/* This instruction is followed by a parameter encoding byte. It         */
/* takes a register and displays the character the ASCII code of         */
/* which is contained in the register. (a modulo 256 is applied to this  */
/* ascii code, the character is displayed on the standard output) Ex:    */
/* ld %52,r3 aff r3 Displays ’*’ on the standard output                  */
/* ********************************************************************* */

int aff(t_stat *stats, t_champion *champ)
{
    char c;
    champ->pc = (champ->pc + 2) % MEM_SIZE;
    if (stats->arena.zone[champ->pc] <= REG_NUMBER)
	{
        c = champ->registers[stats->arena.zone[champ->pc]] % 256;
        write(1, &c, 1);
    }
    champ->pc = (champ->pc + 1) % MEM_SIZE;
    return 0;
}

void refresh_alive(t_stat *stats)
{
	for (int i = 0; i < stats->nb_champions; i++)
		stats->champions[i].is_alive = FALSE;
}

void display_winner(t_stat *stats)
{
	for (int i = 0; i <= stats->nb_champions; i++)
	{
		if (stats->champions[i].number == stats->last_user_live)
			printf("The player %i (%s) has won.\n", stats->last_user_live, stats->champions[i].header.prog_name);
		else if (stats->last_user_live == 0)
		{
			printf("No Winner.\n");
			return;
		}
	}
}

void exec_function(t_stat *stats, t_champion *champ)
{
	if ((stats->arena.zone[champ->pc % MEM_SIZE] >= 0 && stats->arena.zone[champ->pc % MEM_SIZE] <= 15) && champ->living == TRUE && stats->arena.user_id[champ->pc % MEM_SIZE] == champ->number)
	{
		champ->is_waiting = FALSE;
		op_func[stats->arena.zone[champ->pc % MEM_SIZE] - 1](stats, champ);
	} 
	else if (champ->living == TRUE)
		champ->pc = (champ->pc + 1) % MEM_SIZE;
}

int turn(t_stat *stats)
{
	for (int i = 0; i < stats->nb_champions; i++)
	{
		if (stats->champions[i].is_waiting == FALSE && stats->arena.zone[stats->champions[i].pc % MEM_SIZE] > 0 && stats->arena.zone[stats->champions[i].pc % MEM_SIZE] <= 16)
		{
			stats->champions[i].is_waiting = TRUE;
			stats->champions[i].wait = op_tab[stats->arena.zone[stats->champions[i].pc % MEM_SIZE]].nb_cycles;
		} 
		else if (stats->champions[i].is_waiting == TRUE && stats->champions[i].wait == 0 && stats->champions[i].living == TRUE)
			exec_function(stats, &stats->champions[i]);
		else
			stats->champions[i].wait--;
	}
	return 0;
}

int check_alive(t_stat *stats)
{
	int ret = 0;
	for (int i = 0; i < stats->nb_champions; i++)
	{
		if (stats->champions[i].is_alive == TRUE)
			ret++;
		else
			stats->champions[i].living = FALSE;
	}
	return ret;
}

void print_hexarena(t_stat *stats)
{
	for (int i = 0; i < MEM_SIZE; i++)
	{
		if (i % 32 == 0 && i > 0)
		{
			printf("\n");
		}
		printf(stats->arena.zone[i] == 0 ? "0 " : "%X ", (int)stats->arena.zone[i]);
	}
	printf("\n");
}

void loop(t_stat *stats)
{
	int decrement = 1;
	while (stats->arena.cycle_to_die > 0)
	{
		stats->arena.cycle = 0;
		while (stats->arena.cycle <= stats->arena.cycle_to_die)
		{
			if (stats->dump > 0 && stats->arena.cycle >= stats->dump)
				print_hexarena(stats);
			if (stats->nb_live >= NBR_LIVE && check_alive(stats) == stats->nb_champions)
			{
				stats->arena.cycle_to_die -= (CYCLE_DELTA * decrement++);
				stats->nb_live = 0;
			}
			turn(stats);
			stats->arena.cycle++;
		}
		if (check_alive(stats) <= 1)
		{
			display_winner(stats);
			exit(0);
		}
		refresh_alive(stats);
	}
}

void print_arena(t_stat *stats)
{
	int tmp = -1;
	for (int i = 0; i < MEM_SIZE; i++)
	{
		printf(stats->arena.user_id[i] == 0 ? "\e[30m%X\e[0m " : stats->arena.user_id[i] == 1 ? BLUE: stats->arena.user_id[i] == 2 ? RED: stats->arena.user_id[i] == 3 ? YELLOW : GREEN, stats->arena.zone[i]);
		if (i == tmp + 32)
		{
			tmp = i;
			printf("\n");
		}
	}
	printf("\n");
}

int set_arena(t_stat *stats)
{
	int i = 0;
	int j = 0;

	while (i < stats->nb_champions)
	{
		j = 0;
		while (j < stats->champions[i].header.prog_size)
		{
			if (stats->arena.zone[stats->champions[i].load_adress + j] == 0)
			{
				stats->champions[i].pc = stats->champions[i].load_adress;
				stats->arena.zone[stats->champions[i].load_adress + j] = stats->champions[i].code[j];
			}
			else
				return -1;
			stats->arena.user_id[stats->champions[i].load_adress + j] = i + 1;
			j++;
		}
		i++;
	}
	return 0;
}

int init_arena(t_stat *stats)
{
	stats->arena.user_id = malloc(sizeof(int) * MEM_SIZE);
	stats->arena.zone = malloc(sizeof(char) * MEM_SIZE);
	stats->arena.cycle = 0;
	for (int i = 0; i < MEM_SIZE; i++)
	{
		stats->arena.zone[i] = '\0';
		stats->arena.user_id[i] = 0;
	}
	if (set_arena(stats) == -1)
	{
		write(2, "Error: adress\n", 14);
		// return -1;
	}
	stats->arena.cycle_to_die = CYCLE_TO_DIE - stats->dump;
	print_arena(stats);
	print_hexarena(stats);
	return 0;
}

int read_champions(char *path, t_champion *champion)
{
	size_t r = 0;
	int fd = open(path, O_RDONLY);
	if (fd < 0)
		return -1;
	if ((r = read(fd, &champion->header, sizeof(t_header))) <= 0)
		return -1;
	champion->header.magic = SWAP_ENDIAN(champion->header.magic);
	champion->header.prog_size = SWAP_ENDIAN(champion->header.prog_size);
	champion->is_alive = FALSE;
	champion->living = TRUE;
	champion->is_waiting = FALSE;
	champion->wait = 0;
	champion->code = malloc(sizeof(champion->code) * champion->header.prog_size);
	if ((r = read(fd, champion->code, champion->header.prog_size)) < champion->header.prog_size)
		return -1;
	return 0;
}

int check_char(char c, char *str)
{
	for (int i = 0; str[i]; i++)
		if (str[i] == c)
			return 0;
	return -1;
}

int check_all_char(char *str, char *to_check)
{
	for (int i = 0; str[i]; i++)
		if (check_char(str[i], to_check) == -1)
			return -1;
	return 0;
}

int check_cor(char *str)
{
	int i;
	for (i = ft_strlen(str) - 1; i > 0 && str[i] != '.'; i--);
	if (ft_strcmp(&str[i], ".cor") != 0)
		return -1;
	return 0;
}

int parsing_arg_champion(char **argv, t_champion *champ, int argc, int i)
{
	if (ft_strcmp(argv[i], "-n") == 0 && i + 1 < argc)
	{
		if (check_all_char(argv[i + 1], "0123456789") == -1)
			return -1;
		champ->number = ft_atoi(argv[++i]);
		i++;
	}
	if (ft_strcmp(argv[i], "-a") == 0 && i + 1 < argc)
	{
		if (check_all_char(argv[i + 1], "0123456789") == -1)
			return -1;
		champ->load_adress = ft_atoi(argv[++i]);
		i++;
	}
	if (check_cor(argv[i]) == -1)
		return -1;
	champ->registers[1] = champ->number;
	if (read_champions(argv[i], champ) == -1)
		return -1;
	return i;
}

int parsing_arg(int argc, char **argv, t_stat *stats)
{
	int i = 1;
	if (argc > 2 && ft_strcmp(argv[1], "-dump") == 0 && check_all_char(argv[2], "0123456789") == 0) 
	{
		i = 3;
		stats->dump = ft_atoi(argv[2]);
	}
	for (int j = 0; i < argc; i++, j++)
	{
		i = parsing_arg_champion(argv, &stats->champions[j], argc , i);
		if (i == -1)
			return -1;
	}
	return 0;
}

int	init_stats(int argc, char **argv, t_stat *stats)
{
	stats->dump = 0;
	stats->nb_live = 0;
	stats->last_user_live = 0;
	stats->nb_champions = 0;
	for (int j = 1; j < argc; j++)
		if (check_cor(argv[j]) == 0)
			stats->nb_champions++;
	stats->champions = malloc(sizeof(t_champion) * stats->nb_champions);
	for (int i = 0; i < stats->nb_champions; i++)
	{
		stats->champions[i].number = i + 1;
		stats->champions[i].carry = 0;
		stats->champions[i].load_adress = 0;
		stats->champions[i].pc = 0;
		for (int j = 0; j < REG_NUMBER; j++)
			stats->champions[i].registers[j] = 0;
	}
	return 0;
}

int	main(int argc, char **argv)
{
	t_stat stats;
	if (argc == 2 && ft_strcmp(argv[1], "-h") == 0)
		write(1, "Usage : \n", 9);
	if (argc < 3 || init_stats(argc, argv, &stats) == -1 || parsing_arg(argc, argv, &stats) == -1)
		return -1;
	if (init_arena(&stats) == -1)
		return -1;
	loop(&stats);
	return 0;
}
