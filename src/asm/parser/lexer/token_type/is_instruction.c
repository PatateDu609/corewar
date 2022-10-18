#include <libft.h>
#include "asm/parser.h"
#include "common/op.h"

bool is_instruction(const char *val)
{
	return get_op_by_name(val) != NULL;
}
