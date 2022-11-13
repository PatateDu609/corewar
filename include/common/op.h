#ifndef __OP_H__
# define __OP_H__

#include <inttypes.h>
#include <stddef.h>

/*
** Toutes les tailles sont en octets.
** On part du principe qu'un int fait 32 bits. Est-ce vrai chez vous ?
*/

#define REG_SIZE				1
#define IND_SIZE				2
#define DIR_SIZE				4

#if DIR_SIZE == 1
typedef uint8_t dir_t;
#elif DIR_SIZE == 2
typedef uint16_t dir_t;
#elif DIR_SIZE == 4
typedef uint32_t dir_t;
#endif

# define REG_CODE				1
# define DIR_CODE				2
# define IND_CODE				3


#define MAX_ARGS_NUMBER			4
#define MAX_PLAYERS				4
#define MEM_SIZE				(4*1024)
#define IDX_MOD					(MEM_SIZE / 8)
#define CHAMP_MAX_SIZE			(MEM_SIZE / 6)

#define COMMENT_CHAR			'#'
#define LABEL_CHAR				':'
#define DIRECT_CHAR				'%'
#define SEPARATOR_CHAR			','

#define LABEL_CHARS				"abcdefghijklmnopqrstuvwxyz_0123456789"

#define HDR_PREFIX				'.'
#define NAME_CMD_STRING			".name"
#define COMMENT_CMD_STRING		".comment"

#define REG_PREFIX				"r"
#define REG_MIN					1
#define REG_NUMBER				16

#define CYCLE_TO_DIE			1536
#define CYCLE_DELTA				50
#define NBR_LIVE				21
#define MAX_CHECKS				10

/*
**
*/

# define ARGT_REG				1
# define ARGT_DIR				2
# define ARGT_IND				4
# define ARGT_LAB				8

# define PROG_NAME_LENGTH		(128)
# define COMMENT_LENGTH			(2048)
# define COREWAR_EXEC_MAGIC		((uint32_t)0xea83f3)

typedef struct	s_header
{
	unsigned int	magic;
	char			prog_name[PROG_NAME_LENGTH + 1];
	unsigned int	prog_size;
	char			comment[COMMENT_LENGTH + 1];
}	t_header;

typedef struct s_op	t_op;

struct s_op
{
	char*			name;
	int				nb_params;
	int				opcode;
	int				nb_cycles;
	char*			description;
	int				has_pcode;
	int				has_idx;
	int				boolean_length_direct;
	int				param_types[4];
};


typedef enum {
	PARAM_UNKNOWN = 0,
	PARAM_REGISTER = 1,
	PARAM_INDIRECT = 2,
	PARAM_DIRECT = 4,
}								t_param_type;

extern t_op op_tab[17];

t_op *get_op_by_name(const char *tag);

#endif
