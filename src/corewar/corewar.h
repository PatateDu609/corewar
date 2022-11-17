/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   corewar.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbourgea <rbourgea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 14:42:52 by rbourgea          #+#    #+#             */
/*   Updated: 2022/11/17 14:27:05 by rbourgea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COREWAR_H
# define COREWAR_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "op.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// #include "../../lib/libft/include/libft.h"

#define FALSE 0
#define TRUE 1

#define BLACK    "\e[30m%02X\e[0m "
#define RED      "\e[31m%02X\e[0m "
#define GREEN    "\e[32m%02X\e[0m "
#define YELLOW   "\e[33m%02X\e[0m "
#define BLUE     "\e[34m%02X\e[0m "

#define SWAP_ENDIAN(x) ((((x>>24) & 0xff) | ((x<<8) & 0xff0000)) | (((x>>8) & 0xff00) | ((x<<24) & 0xff000000)))
#define SWAP_ENDIAN_16(x) (((x>>24) & 0xff) | ((x<<8) & 0xff00))

#define REGISTER(c) ((c & 0xC0) == 0x40 ? 1 : 0)
#define INDIRECT(c) ((c & 0xC0) == 0xC0 ? 1 : 0)
#define DIRECT(c) ((c & 0xC0) == 0x80 ? 1 : 0)

typedef struct	s_champion
{
	int number;
	int load_adress;
	t_header header;
	int registers[REG_NUMBER + 1];
	int pc;
	int carry;
	int is_alive;
	int living;
	int is_waiting;
	int wait;
	unsigned char *code;
}				t_champion;

typedef struct	s_arena
{
	unsigned char *zone;
	int *user_id;
	int cycle_to_die;
	int cycle;
}				t_arena;

typedef struct	s_stat
{
	int dump;
	int nb_champions;
	int nb_live;
	int last_user_live;
	t_champion *champions;
	t_arena arena;
}				t_stat;

int nothing(t_stat *stats, t_champion *champ);
int live(t_stat *stats, t_champion *champ);
int ld(t_stat *stats, t_champion *champ);
int st(t_stat *stats, t_champion *champ);
int add(t_stat *stats, t_champion *champ);
int sub(t_stat *stats, t_champion *champ);
int get_type(char code, t_champion *champ, t_stat *stats);
int and(t_stat *stats, t_champion *champ);
int or(t_stat *stats, t_champion *champ);
int xor(t_stat *stats, t_champion *champ);
int zjump(t_stat *stats, t_champion *champ);
int ldi(t_stat *stats, t_champion *champ);
int get_index_or_register(t_stat *stats, t_champion *champ, int code, int save);
int sti(t_stat *stats, t_champion *champ);
t_champion copy_champ(t_champion *champ);
int get_index(t_stat *stats, t_champion *champ);
t_champion *realloc_champ(t_stat *stats, int index, int offset);
int ft_fork(t_stat *stats, t_champion *champ);
int lld(t_stat *stats, t_champion *champ);
int lldi(t_stat *stats, t_champion *champ);
t_champion *lrealloc_champ(t_stat *stats, int index, int offset);
int lfork(t_stat *stats, t_champion *champ);
int aff(t_stat *stats, t_champion *champ);
void refresh_alive(t_stat *stats);
void display_winner(t_stat *stats);
void exec_function(t_stat *stats, t_champion *champ);
int turn(t_stat *stats);
int check_alive(t_stat *stats);

#endif