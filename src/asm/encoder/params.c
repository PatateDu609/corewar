#include "asm/encoder.h"
#include "asm/ast.h"
#include <libft.h>

#define ADD_DIR(bits) \
    union dir##bits      \
    {                     \
        int##bits##_t val;   \
        uint8_t raw[bits / 8];\
    };

ADD_DIR(16)
ADD_DIR(32)

static uint32_t to_encoded(int32_t val, struct encoder *enc, bool ind, uint32_t offset)
{
	if (ind)
	{
		union dir16 dir = { .val = (int16_t) val };
		dir.val = bswap_16(dir.val);
		ft_memcpy(enc->result->assembly + offset, dir.raw, 2);
		return 2;
	}

	union dir32 dir = { .val = val };
	dir.val = bswap_32(dir.val);
	ft_memcpy(enc->result->assembly + offset, dir.raw, 4);
	return 4;
}

static int32_t get_value(ast_t *param, struct encoder *enc)
{
	int32_t val = 0;
	if (param->childs[0]->type == AST_LABEL)
	{
		struct label *label = lookup_label(enc, param->childs[0]->childs[0]->token->value);
		if (label)
			val = label->off;
	}
	else
		val = parse_number(param->childs[0]->token->value);
	return val;
}

uint32_t dump_direct_param(ast_t *param, struct encoder *enc, t_op *op, uint32_t local_offset, uint32_t offset)
{
	int32_t val = get_value(param, enc);
	if (param->childs[0]->type == AST_LABEL)
		val -= (int32_t)offset;
	return to_encoded(val, enc, op->boolean_length_direct, local_offset + offset);
}

void dump_indirect_param(ast_t *param, struct encoder *enc, uint32_t local_offset, uint32_t offset)
{
	int32_t val = get_value(param, enc);

	to_encoded((int32_t)offset - val, enc, 1, local_offset + offset);
}