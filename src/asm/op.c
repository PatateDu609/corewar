#include <common/op.h>
#include <libft.h>

t_op op_tab[17];

static void set_op_tab(void) __attribute__(( constructor ));

void set_op_tab(void)
{
	t_op live = {
		.name = "live",
		.nb_params = 1,
		.opcode = 1,
		.nb_cycles = 10,
		.description = "alive",
		.has_pcode = 0,
		.has_idx = 0,
		.boolean_length_direct = 0,
		.param_types = {PARAM_DIRECT},
	};
	t_op ld = {
		.name = "ld",
		.nb_params = 2,
		.opcode = 2,
		.nb_cycles = 5,
		.description = "load",
		.has_pcode = 1,
		.has_idx = 0,
		.boolean_length_direct = 0,
		.param_types = {PARAM_DIRECT | PARAM_INDIRECT, PARAM_REGISTER},
	};
	t_op st = {
		.name = "st",
		.nb_params = 2,
		.opcode = 3,
		.nb_cycles = 5,
		.description = "store",
		.has_pcode = 1,
		.has_idx = 0,
		.boolean_length_direct = 0,
		.param_types = {PARAM_REGISTER, PARAM_INDIRECT | PARAM_REGISTER},
	};
	t_op add = {
		.name = "add",
		.nb_params = 3,
		.opcode = 4,
		.nb_cycles = 10,
		.description = "addition",
		.has_pcode = 1,
		.has_idx = 0,
		.boolean_length_direct = 0,
		.param_types = {PARAM_REGISTER, PARAM_REGISTER, PARAM_REGISTER},
	};
	t_op sub = {
		.name = "sub",
		.nb_params = 3,
		.opcode = 5,
		.nb_cycles = 10,
		.description = "soustraction",
		.has_pcode = 1,
		.has_idx = 0,
		.boolean_length_direct = 0,
		.param_types = {PARAM_REGISTER, PARAM_REGISTER, PARAM_REGISTER},
	};
	t_op and = {
		.name = "and",
		.nb_params = 3,
		.opcode = 6,
		.nb_cycles = 6,
		.description = "et (and  r1, r2, r3   r1&r2 -> r3",
		.has_pcode = 1,
		.has_idx = 0,
		.boolean_length_direct = 0,
		.param_types = {PARAM_REGISTER | PARAM_DIRECT | PARAM_INDIRECT, PARAM_REGISTER | PARAM_INDIRECT | PARAM_DIRECT, PARAM_REGISTER},
	};
	t_op or = {
		.name = "or",
		.nb_params = 3,
		.opcode = 7,
		.nb_cycles = 6,
		.description = "ou  (or   r1, r2, r3   r1 | r2 -> r3",
		.has_pcode = 1,
		.has_idx = 0,
		.boolean_length_direct = 0,
		.param_types = {PARAM_REGISTER | PARAM_INDIRECT | PARAM_DIRECT, PARAM_REGISTER | PARAM_INDIRECT | PARAM_DIRECT, PARAM_REGISTER},
	};
	t_op xor = {
		.name = "xor",
		.nb_params = 3,
		.opcode = 8,
		.nb_cycles = 6,
		.description = "ou (xor  r1, r2, r3   r1^r2 -> r3",
		.has_pcode = 1,
		.has_idx = 0,
		.boolean_length_direct = 0,
		.param_types = {PARAM_REGISTER | PARAM_INDIRECT | PARAM_DIRECT, PARAM_REGISTER | PARAM_INDIRECT | PARAM_DIRECT, PARAM_REGISTER},
	};
	t_op zjmp = {
		.name = "zjmp",
		.nb_params = 1,
		.opcode = 9,
		.nb_cycles = 20,
		.description = "jump if zero",
		.has_pcode = 0,
		.has_idx = 1,
		.boolean_length_direct = 1,
		.param_types = {PARAM_DIRECT},
	};
	t_op ldi = {
		.name = "ldi",
		.nb_params = 3,
		.opcode = 10,
		.nb_cycles = 25,
		.description = "load index",
		.has_pcode = 1,
		.has_idx = 1,
		.boolean_length_direct = 1,
		.param_types = {PARAM_REGISTER | PARAM_DIRECT | PARAM_INDIRECT, PARAM_DIRECT | PARAM_REGISTER, PARAM_REGISTER},
	};
	t_op sti = {
		.name = "sti",
		.nb_params = 3,
		.opcode = 11,
		.nb_cycles = 25,
		.description = "store index",
		.has_pcode = 1,
		.has_idx = 1,
		.boolean_length_direct = 1,
		.param_types = {PARAM_REGISTER, PARAM_REGISTER | PARAM_DIRECT | PARAM_INDIRECT, PARAM_DIRECT | PARAM_REGISTER},
	};
	t_op fork = {
		.name = "fork",
		.nb_params = 1,
		.opcode = 12,
		.nb_cycles = 800,
		.description = "fork",
		.has_pcode = 0,
		.has_idx = 1,
		.boolean_length_direct = 1,
		.param_types = {PARAM_DIRECT},
	};
	t_op lld = {
		.name = "lld",
		.nb_params = 2,
		.opcode = 13,
		.nb_cycles = 10,
		.description = "long load",
		.has_pcode = 1,
		.has_idx = 0,
		.boolean_length_direct = 0,
		.param_types = {PARAM_DIRECT | PARAM_INDIRECT, PARAM_REGISTER},
	};
	t_op lldi = {
		.name = "lldi",
		.nb_params = 3,
		.opcode = 14,
		.nb_cycles = 50,
		.description = "long load index",
		.has_pcode = 1,
		.has_idx = 1,
		.boolean_length_direct = 1,
		.param_types = {PARAM_REGISTER | PARAM_DIRECT | PARAM_INDIRECT, PARAM_DIRECT | PARAM_REGISTER, PARAM_REGISTER},
	};
	t_op lfork = {
		.name = "lfork",
		.nb_params = 1,
		.opcode = 15,
		.nb_cycles = 1000,
		.description = "long fork",
		.has_pcode = 0,
		.has_idx = 1,
		.boolean_length_direct = 1,
		.param_types = {PARAM_DIRECT},
	};
	t_op aff = {
		.name = "aff",
		.nb_params = 1,
		.opcode = 16,
		.nb_cycles = 2,
		.description = "aff",
		.has_pcode = 1,
		.has_idx = 0,
		.boolean_length_direct = 0,
		.param_types = {PARAM_REGISTER},
	};

	op_tab[0] = (t_op){ "", 0, 0, 0, NULL, 0, 0, 0, {0} };
	op_tab[1] = live;
	op_tab[2] = ld;
	op_tab[3] = st;
	op_tab[4] = add;
	op_tab[5] = sub;
	op_tab[6] = and;
	op_tab[7] = or;
	op_tab[8] = xor;
	op_tab[9] = zjmp;
	op_tab[10] = ldi;
	op_tab[11] = sti;
	op_tab[12] = fork;
	op_tab[13] = lld;
	op_tab[14] = lldi;
	op_tab[15] = lfork;
	op_tab[16] = aff;
}

t_op *get_op_by_name(const char *tag)
{
	static size_t size = sizeof op_tab / sizeof *op_tab;

	for (size_t i = 0; i < size; i++)
		if (!ft_strcmp(tag, op_tab[i].name))
			return op_tab + i;
	return NULL;
}