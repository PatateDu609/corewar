#include "asm/encoder.h"
#include "asm/ast.h"

static size_t get_param_size(enum ast_node_type ast_type, bool dir_len)
{
	switch (ast_type)
	{
	case AST_PARAM_REGISTER:
		return REG_SIZE;
	case AST_PARAM_INDIRECT:
		return IND_SIZE;
	case AST_PARAM_DIRECT:
		return dir_len ? IND_SIZE : DIR_SIZE;
	default:
		__builtin_unreachable();
		break;
	}
}

static size_t get_pcode_size(t_op *op)
{
	size_t size_acb = 0;

	if (op->has_pcode)
	{
		size_acb = op->nb_params * 2;
		size_acb = (size_acb + (size_acb % 8)) / 8;
	}
	return size_acb;
}

size_t get_instr_size(ast_t *ast)
{
	if (ast->type != AST_INSTRUCTION)
		return 0;
	size_t size = 1; // Opcode size

	t_op *op = get_op_by_name(ast->childs[0]->token->value);
	size += get_pcode_size(op);

	ast = ast->childs[1];
	for (size_t i = 0; i < ast->nb_childs; i++)
		size += get_param_size(ast->childs[i]->type, op->boolean_length_direct);
	return size;
}