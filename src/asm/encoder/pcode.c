#include "asm/encoder.h"
#include "asm/ast.h"
#include "common/op.h"

size_t get_pcode_size(t_op *op)
{
	size_t size_acb = 0;

	if (op->has_pcode)
	{
		size_acb = op->nb_params * 2;
		size_acb = (size_acb + (size_acb % 8)) / 8;
	}
	return size_acb;
}

static uint8_t instr_type_to_pcode(enum ast_node_type type)
{
	switch (type)
	{
	case AST_PARAM_REGISTER:
		return REG_CODE;
	case AST_PARAM_INDIRECT:
		return IND_CODE;
	case AST_PARAM_DIRECT:
		return DIR_CODE;
	default:
		return 0; // unknown type...
	}
}

uint32_t get_pcode(uint8_t *encoded, ast_t *params, uint32_t local_offset)
{
	size_t i = 0;
	for (; i < params->nb_childs; i++)
	{
		encoded[local_offset] =
			(encoded[local_offset] << 2) |
			(instr_type_to_pcode(params->childs[i]->type) & 0b11);

		if ((i + 1) % 4 == 0)
			local_offset++;
	}
	i %= 4;
	encoded[local_offset++] <<= (4 - i) * 2;

	return local_offset;
}