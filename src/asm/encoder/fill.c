#include <libft.h>
#include "asm/encoder.h"
#include "asm/ast.h"

static uint32_t encode_instruction(ast_t *instr, struct encoder *enc, uint32_t offset)
{
	uint32_t local_offset = 0;
	ast_t *params = instr->childs[1];
	uint8_t *encoded = enc->result->assembly + offset;

	t_op *op = get_op_by_name(instr->childs[0]->token->value);
	encoded[local_offset++] = op->opcode;
	if (op->has_pcode)
		local_offset = get_pcode(encoded, params, local_offset);

	for (size_t i = 0; i < params->nb_childs; i++)
	{
		ast_t *param = params->childs[i];

		if (param->type == AST_PARAM_REGISTER)
			encoded[local_offset++] = ft_atoi(param->childs[0]->token->value + 1);
		else if (param->type == AST_PARAM_DIRECT)
			local_offset += dump_direct_param(param, enc, op, local_offset, offset);
		else if (param->type == AST_PARAM_INDIRECT)
		{
			dump_indirect_param(param, enc, local_offset, offset);
			local_offset += 2;
		}
	}

	return local_offset;
}

void fill_encoder(struct encoder *enc)
{
	uint32_t offset = 0;

	for (size_t i = 0; i < enc->p->nb; i++)
	{
		ast_t *ast = enc->p->lns[i].ast;

		ast_t *instr = ast->childs[ast->nb_childs - 1];
		if (instr->type != AST_INSTRUCTION) // it may be a label alone or a header
			continue;
		offset += encode_instruction(instr, enc, offset);
	}
}